/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:04:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/06 12:02:39 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_routine.h"

void print_log(char *string, t_coder *coder)
{
	long	time;
	
	pthread_mutex_lock(coder->monitor->monitor_mutex);
	time = ft_get_time() - coder->monitor->start_time;
	pthread_mutex_unlock(coder->monitor->monitor_mutex);
	pthread_mutex_lock(coder->monitor->print_mutex);
	printf("%li %i %s\n", time, coder->id, string);
	pthread_mutex_unlock(coder->monitor->print_mutex);
}

int	dongle_is_avaible(t_coder *coder, int compile_count)
{
	pthread_mutex_lock(coder->left_dongle->dongle_mutex);
	pthread_mutex_lock(coder->right_dongle->dongle_mutex);
	if (compile_count == 0 && coder->left_dongle->is_free == true && coder->right_dongle->is_free == true)
	{
		pthread_mutex_unlock(coder->left_dongle->dongle_mutex);
		pthread_mutex_unlock(coder->right_dongle->dongle_mutex);	
		return (0);
	}
	if (coder->left_dongle->is_free == true &&
		coder->left_dongle->is_free == true &&
		ft_get_time() - coder->monitor->last_dongle_release > coder->monitor->dongle_cooldown)
	{
		pthread_mutex_unlock(coder->left_dongle->dongle_mutex);
		pthread_mutex_unlock(coder->right_dongle->dongle_mutex);	
		return (0);
	}
	pthread_mutex_unlock(coder->left_dongle->dongle_mutex);
	pthread_mutex_unlock(coder->right_dongle->dongle_mutex);	
	return (1);
}

void	coder_action(t_coder *coder)
{
	int		compile_count;
	long	time_to_compile;
	long	time_to_refactor;
	long	time_to_debug;
	int		compiling_nb;
	
	pthread_mutex_lock(coder->monitor->monitor_mutex);
	time_to_compile = coder->monitor->time_to_compile;
	time_to_refactor = coder->monitor->time_to_refactor;
	time_to_debug = coder->monitor->time_to_debug;
	compiling_nb = coder->monitor->compiling_nb;
	compile_count = coder->compile_count;
	pthread_mutex_unlock(coder->monitor->monitor_mutex);
	
	if (compile_count == compiling_nb)
	{
		pthread_mutex_lock(coder->monitor->monitor_mutex);
		coder->monitor->status = "FINISH";
		pthread_mutex_unlock(coder->monitor->monitor_mutex);
		return;
	}

	if (dongle_is_avaible(coder, compile_count) == 1)
		return;

	pthread_mutex_lock(coder->left_dongle->dongle_mutex);
	coder->left_dongle->is_free = false;
	pthread_mutex_unlock(coder->left_dongle->dongle_mutex);
	print_log("has taken a dongle", coder);

	pthread_mutex_lock(coder->right_dongle->dongle_mutex);
	coder->right_dongle->is_free = false;
	pthread_mutex_unlock(coder->right_dongle->dongle_mutex);
	print_log("has taken a dongle", coder);

	print_log("is compiling", coder);

	pthread_mutex_lock(coder->monitor->monitor_mutex);
	usleep(time_to_compile);
	coder->compile_count++;
	pthread_mutex_unlock(coder->monitor->monitor_mutex);

	pthread_mutex_lock(coder->monitor->monitor_mutex);
	coder->monitor->last_compile = ft_get_time();
	coder->monitor->last_dongle_release = ft_get_time();
	pthread_mutex_unlock(coder->monitor->monitor_mutex);

	pthread_mutex_lock(coder->left_dongle->dongle_mutex);
	pthread_mutex_lock(coder->right_dongle->dongle_mutex);
	coder->right_dongle->is_free = true;
	coder->left_dongle->is_free = true;
	pthread_mutex_unlock(coder->left_dongle->dongle_mutex);
	pthread_mutex_unlock(coder->right_dongle->dongle_mutex);

	print_log("is debugging", coder);
	usleep(time_to_debug);

	print_log("is refactoring", coder);
	usleep(time_to_refactor);

}

void	*coder_routine(void *v_coder)
{
	t_coder	*coder;
	int		i;
	int		burned_out;

	i = 0;
	coder = (t_coder*)v_coder;
	// printf("coder %i start his routine and wait\n", coder->id);
	pthread_mutex_lock(coder->monitor->monitor_mutex);
	coder->status = "READY";
	while (strcmp(coder->monitor->status, "READY") != 0)
	{
		pthread_cond_wait(coder->monitor->monitor_cond, coder->monitor->monitor_mutex);
	}
	if (coder->id % 2 == 0)
		usleep(1000);
	pthread_mutex_unlock(coder->monitor->monitor_mutex);
	// printf("coder %i finish waiting\n", coder->id);
	// printf("coder %i start routine\n", coder->id);
	while (1)
	{
		pthread_mutex_lock(coder->monitor->monitor_mutex);
		if(strcmp(coder->monitor->status, "BURNOUT") == 0 || strcmp(coder->monitor->status, "FINISH") == 0)
		{
			pthread_mutex_unlock(coder->monitor->monitor_mutex);
			break;
		}
		pthread_mutex_unlock(coder->monitor->monitor_mutex);
		coder_action(coder);
		usleep(200);
	}

	pthread_mutex_lock(coder->monitor->monitor_mutex);
	if (strcmp(coder->monitor->status, "BURNOUT") == 0)
		burned_out = 1;
	pthread_mutex_unlock(coder->monitor->monitor_mutex);

	if (burned_out == 1)
		print_log("burned out", coder);
	return (NULL);
}

// faire en sorte que N sits between coder number N - 1 and coder number N + 1.

// tester avec 3 coders et 

//FAIT creer nb_coders dongle FAIT
// ckeck si burned out : afficher:  timestamp_in_ms X burned out (geré par le monitor)
	// faire number_of_compiles_required fois: 
		// faire partir les coders pair avec un dongle que apres (selon le scheduler):
		// 1 coder choppe 2 dongles  selon le cooldown et selon le scheduler !!!!!
		// afficher: timestamp_in_ms X has taken a dongle x2
		// afficher: timestamp_in_ms is compiling (pendant time_to_compile ms)
		// lacher les 2 dongles et reacutalise le temps ou tu les lache
		// afficher: timestamp_in_ms is debugging (pendant time_to_debug ms)
		// afficher: timestamp_in_ms is refactoring (pendant time_to_refactor ms)


// schedluer : FIFO (..)
//			   EDF  (Eraliest deadline first: last_compile_start + time_to_burnout)

