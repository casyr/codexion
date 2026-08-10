/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:04:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/10 19:05:03 by yriffard         ###   ########.fr       */
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

int	dongle_is_avaible(t_dongle *first_dongle, t_dongle *second_dongle,  t_coder *coder, int compile_count)
{
	// printf("left: %p", coder->left_dongle->dongle_mutex);
	// printf("right: %p", coder->right_dongle->dongle_mutex);
	pthread_mutex_lock(first_dongle->dongle_mutex);
	pthread_mutex_lock(second_dongle->dongle_mutex);
	if (compile_count == 0 && coder->left_dongle->is_free == true && coder->right_dongle->is_free == true)
	{
		pthread_mutex_unlock(coder->left_dongle->dongle_mutex);
		pthread_mutex_unlock(coder->right_dongle->dongle_mutex);
		return (0);
	}
	if (coder->left_dongle->is_free == true &&
		coder->right_dongle->is_free == true &&
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
	int		target_compiling_nb;
	t_dongle	*first_dongle;
	t_dongle	*second_dongle;

	pthread_mutex_lock(coder->monitor->monitor_mutex);
	time_to_compile = coder->monitor->time_to_compile;
	time_to_refactor = coder->monitor->time_to_refactor;
	time_to_debug = coder->monitor->time_to_debug;
	target_compiling_nb = coder->monitor->compiling_nb;
	compile_count = coder->compile_count;
	
	pthread_mutex_unlock(coder->monitor->monitor_mutex);

	if (coder->left_dongle->dongle_mutex < coder->right_dongle->dongle_mutex)
	{
		first_dongle = coder->left_dongle;
		second_dongle = coder->right_dongle;
	}
	else
	{
		first_dongle = coder->right_dongle;
		second_dongle = coder->left_dongle;
	}
	// printf("%i, %i\n", compile_count, compiling_nb);
	if (compile_count >= target_compiling_nb)
	{
		pthread_mutex_lock(coder->monitor->monitor_mutex);
		coder->status = "FINISH";
		coder->monitor->finished_coders_nb += 1;
		pthread_mutex_unlock(coder->monitor->monitor_mutex);
		return;
	}
	// printf("%i,\n", dongle_is_avaible(coder, compile_count));
	if (dongle_is_avaible(first_dongle, second_dongle, coder, compile_count) == 1)
		return;

	pthread_mutex_lock(first_dongle->dongle_mutex);
	pthread_mutex_lock(second_dongle->dongle_mutex);
	first_dongle->is_free = false;

	print_log("has taken a dongle", coder);


	second_dongle->is_free = false;

	print_log("has taken a dongle", coder);

	print_log("is compiling", coder);

	usleep(time_to_compile);

	coder->compile_count++;
	// printf("coder id: %i, compile nb: %i, finished coder nb %i\n", coder->id, coder->compile_count, coder->monitor->finished_coders_nb);

	coder->monitor->last_compile = ft_get_time();
	coder->monitor->last_dongle_release = ft_get_time();
	
	first_dongle->is_free = true;
	second_dongle->is_free = true;
	pthread_mutex_unlock(first_dongle->dongle_mutex);
	pthread_mutex_unlock(second_dongle->dongle_mutex);

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
	burned_out = 0;
	coder = (t_coder*)v_coder;
	// printf("coder %i start his routine and wait\n", coder->id);
	pthread_mutex_lock(coder->monitor->monitor_mutex);
	coder->status = "READY";
	while (strcmp(coder->monitor->status, "READY") != 0)
	{
		if (strcmp(coder->monitor->status, "BURNOUT") == 0)
		{
			pthread_mutex_unlock(coder->monitor->monitor_mutex);
			return (NULL);
		}
		// printf("wait\n");
		pthread_cond_wait(coder->monitor->monitor_cond, coder->monitor->monitor_mutex);
	}
	pthread_mutex_unlock(coder->monitor->monitor_mutex);
	if (coder->id % 2 == 0)
		usleep(1000);
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

