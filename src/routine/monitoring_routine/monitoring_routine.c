/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:02:13 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/20 21:44:36 by yriffard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "monitoring.h"

void	burn_out_loop_checker(t_monitoring *monitor, long time)
{
	int	i;

	i = 0;
	while (i < monitor->coders_nb)
	{
		if ((time - monitor->coder_list[i].last_compile)
			> monitor->time_to_burnout)
		{
			monitor->status = "BURNOUT";
			pthread_mutex_lock(&(monitor->print_mutex));
			printf("%li %i burned out\n",
				time - monitor->start_time, monitor->coder_list[i].id);
			pthread_mutex_unlock(&(monitor->print_mutex));
			break ;
		}
		i++;
	}
}

void	monitoring_loop(t_monitoring *monitor)
{
	while (1)
	{
		pthread_mutex_lock(&(monitor->monitor_mutex));
		if (monitor->finished_coders_nb >= monitor->coders_nb)
		{
			monitor->status = "FINISH";
			pthread_mutex_unlock(&(monitor->monitor_mutex));
			break ;
		}
		burn_out_loop_checker(monitor, ft_get_time());
		if (strcmp(monitor->status, "BURNOUT") == 0)
		{
			pthread_mutex_unlock(&(monitor->monitor_mutex));
			break ;
		}
		pthread_mutex_unlock(&(monitor->monitor_mutex));
		usleep(100);
	}
}

int	coder_are_ready(t_monitoring *monitor)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < monitor->coders_nb)
	{
		if (strcmp(monitor->coder_list[i].status, "READY") == 0)
			count++;
		i++;
	}
	return (count == monitor->coders_nb);
}

void	burned_out_case(t_monitoring *monitor)
{
	pthread_mutex_lock(&(monitor->print_mutex));
	printf("%li %i burned out\n", (ft_get_time()
			- monitor->start_time), monitor->coder_list[0].id);
	pthread_mutex_unlock(&(monitor->print_mutex));
	pthread_mutex_unlock(&(monitor->monitor_mutex));
}

void	*monitoring_routine(void *monitoring)
{
	t_monitoring	*monitor;

	monitor = (t_monitoring *) monitoring;
	while (1)
	{
		pthread_mutex_lock(&(monitor->monitor_mutex));
		if (coder_are_ready(monitor))
		{
			monitor->status = "READY";
			pthread_mutex_unlock(&(monitor->monitor_mutex));
			break ;
		}
		if (strcmp(monitor->status, "BURNOUT") == 0)
		{
			burned_out_case(monitor);
			return (NULL);
		}
		pthread_mutex_unlock(&(monitor->monitor_mutex));
		usleep(200);
	}
	pthread_mutex_lock(&(monitor->monitor_mutex));
	pthread_cond_broadcast(&(monitor->monitor_cond));
	pthread_mutex_unlock(&(monitor->monitor_mutex));
	monitoring_loop(monitor);
	return (NULL);
}
