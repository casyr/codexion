/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:04:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/11 17:16:31 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_routine.h"

int	dongles_are_avaible(t_dongle *first_dongle, t_dongle *second_dongle,  t_coder *coder)
{
	// printf("left: %p", coder->left_dongle->dongle_mutex);
	// printf("right: %p", coder->right_dongle->dongle_mutex);
	pthread_mutex_lock(first_dongle->dongle_mutex);
	pthread_mutex_lock(second_dongle->dongle_mutex);
	if (coder->monitor->total_compile_counter == 0 && coder->left_dongle->is_free == true && coder->right_dongle->is_free == true)
		return (0);
	if (coder->left_dongle->is_free == true &&
		coder->right_dongle->is_free == true &&
		ft_get_time() - coder->right_dongle->last_release > coder->monitor->dongle_cooldown &&
		ft_get_time() - coder->left_dongle->last_release > coder->monitor->dongle_cooldown)
		return (0);
	pthread_mutex_unlock(first_dongle->dongle_mutex);
	pthread_mutex_unlock(second_dongle->dongle_mutex);
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

	// printf("coder id: %i, compile nb: %i, finished coder nb %i, monitor status: %s\n", coder->id, coder->compile_count, coder->monitor->finished_coders_nb, coder->monitor->status);

	if (strcmp(coder->status, "FINISH") == 0)
	{
		usleep(200);
		return;
	}

	if (compile_count == target_compiling_nb)
	{
		pthread_mutex_lock(coder->monitor->monitor_mutex);
		coder->status = "FINISH";
		coder->monitor->finished_coders_nb += 1;
		pthread_mutex_unlock(coder->monitor->monitor_mutex);
		return;
	}
	
	// printf("first_dongle hable to copile in %li\n", (first_dongle->last_release + coder->monitor->dongle_cooldown) - ft_get_time());
	// printf("second hable to copile in %li\n", (second_dongle->last_release + coder->monitor->dongle_cooldown) - ft_get_time());
	// printf("%i,\n", dongle_is_avaible(coder, compile_count));
	if (dongles_are_avaible(first_dongle, second_dongle, coder) == 1)
		return;

	first_dongle->is_free = false;
	print_log("has taken a dongle", coder);

	second_dongle->is_free = false;
	print_log("has taken a dongle", coder);


	print_log("is compiling", coder);
	ft_usleep(time_to_compile, coder->monitor);
	coder->compile_count++;
	coder->monitor->total_compile_counter ++;

	pthread_mutex_lock(coder->monitor->monitor_mutex);
	coder->last_compile = ft_get_time();
	coder->monitor->last_dongle_release = ft_get_time(); ////// supprrr???
	pthread_mutex_unlock(coder->monitor->monitor_mutex);
	
	first_dongle->is_free = true;
	second_dongle->is_free = true;
	coder->left_dongle->last_release = ft_get_time();
	coder->right_dongle->last_release = ft_get_time();
	pthread_mutex_unlock(first_dongle->dongle_mutex);
	pthread_mutex_unlock(second_dongle->dongle_mutex);

	print_log("is debugging", coder);
	ft_usleep(time_to_debug, coder->monitor);

	print_log("is refactoring", coder);
	ft_usleep(time_to_refactor, coder->monitor);
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
		usleep(200);
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
	return (NULL);
}

// schedluer : FIFO (..)
//			   EDF  (Eraliest deadline first: last_compile_start + time_to_burnout)

