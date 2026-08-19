/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:04:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/19 15:40:50 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	coder_compiling(t_coder *coder, int time_to_compile,
	t_dongle *first_dongle, t_dongle *second_dongle)
{
	pthread_mutex_lock(&(coder->monitor->monitor_mutex));
	coder->last_compile = ft_get_time();
	pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
	print_log("is compiling", coder);
	ft_usleep(time_to_compile, coder->monitor);
	pthread_mutex_lock(&(coder->monitor->monitor_mutex));
	coder->compile_count++;
	coder->monitor->total_compile_counter++;
	coder->monitor->last_dongle_release = ft_get_time();
	coder->left_dongle->last_release = ft_get_time();
	coder->right_dongle->last_release = ft_get_time();
	first_dongle->is_free = true;
	second_dongle->is_free = true;
	pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
}

void	coder_action(t_coder *coder, t_dongle *first_dongle,
		t_dongle *second_dongle)
{
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;

	pthread_mutex_lock(&(coder->monitor->monitor_mutex));
	time_to_compile = coder->monitor->time_to_compile;
	time_to_debug = coder->monitor->time_to_debug;
	time_to_refactor = coder->monitor->time_to_refactor;
	pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
	print_log("has taken a dongle", coder);
	print_log("has taken a dongle", coder);
	coder_compiling(coder, time_to_compile, first_dongle, second_dongle);
	print_log("is debugging", coder);
	ft_usleep(time_to_debug, coder->monitor);
	print_log("is refactoring", coder);
	ft_usleep(time_to_refactor, coder->monitor);
}

void	coder_handling(t_coder *coder)
{
	int			compile_count;
	int			target_compiling_nb;
	t_dongle	*first_dongle;
	t_dongle	*second_dongle;

	pthread_mutex_lock(&(coder->monitor->monitor_mutex));
	if (strcmp(coder->status, "FINISH") == 0)
	{
		pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
		usleep(200);
		return ;
	}
	target_compiling_nb = coder->monitor->compiling_nb;
	compile_count = coder->compile_count;
	pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
	first_dongle = first_dongle_chooser(coder);
	second_dongle = second_dongle_chooser(coder);
	if (compile_count == target_compiling_nb)
	{
		coder_set_finish(coder);
		return ;
	}
	if (dongles_are_available(first_dongle, second_dongle, coder) == 1)
		return ;
	coder_action(coder, first_dongle, second_dongle);
}

void	coder_routine_start(t_coder *coder)
{
	while (1)
	{
		pthread_mutex_lock(&(coder->monitor->monitor_mutex));
		if (strcmp(coder->monitor->status, "BURNOUT") == 0
			|| strcmp(coder->monitor->status, "FINISH") == 0)
		{
			pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
			break ;
		}
		pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
		coder_handling(coder);
		usleep(200);
	}
}

void	*coder_routine(void *v_coder)
{
	t_coder	*coder;

	coder = (t_coder *)v_coder;
	pthread_mutex_lock(&(coder->monitor->monitor_mutex));
	if (coder->monitor->coders_nb == 1)
	{
		coder->monitor->status = "BURNOUT";
		pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
		return (NULL);
	}
	coder->status = "READY";
	while (strcmp(coder->monitor->status, "READY") != 0)
	{
		if (strcmp(coder->monitor->status, "BURNOUT") == 0)
		{
			pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
			return (NULL);
		}
		// printf("coder %i is waiting\n", coder->id);
		pthread_cond_wait(&(coder->monitor->monitor_cond),
			&(coder->monitor->monitor_mutex));
	}
	pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
	coder_bump(coder);
	coder_routine_start(coder);
	return (NULL);
}
