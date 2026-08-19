/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:02:13 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/19 15:23:43 by yriffard         ###   ########.fr       */
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
			> monitor->time_to_burnout || monitor->coders_nb == 1)
		{
			monitor->status = "BURNOUT";
			printf("%li %i burned out\n",
				time - monitor->start_time, monitor->coder_list[i].id);
			pthread_mutex_unlock(&(monitor->monitor_mutex));
			break ;
		}
		i++;
	}
}

void	monitoring_loop(t_monitoring *monitor)
{
	long	time;

	while (1)
	{
		time = ft_get_time();
		if (time == 1)
		{
			printf("gettimeofday fail in monitor rountine");
			break ;
		}
		pthread_mutex_lock(&(monitor->monitor_mutex));
		if (monitor->finished_coders_nb >= monitor->coders_nb)
		{
			monitor->status = "FINISH";
			pthread_mutex_unlock(&(monitor->monitor_mutex));
			break ;
		}
		burn_out_loop_checker(monitor, time);
		if (strcmp(monitor->status, "BURNOUT") == 0)
			break ;
		pthread_mutex_unlock(&(monitor->monitor_mutex));
		usleep(200);
	}
}

void	*monitoring_routine(void *monitoring)
{
	t_monitoring	*monitor;
	long			time;

	monitor = (t_monitoring *) monitoring;
	pthread_mutex_lock(&(monitor->monitor_mutex));
	while (strcmp(monitor->status, "READY") != 0)
	{
		if (strcmp(monitor->status, "BURNOUT") == 0)
		{
			time = ft_get_time();
			printf("%li %i burned out\n",
				time - monitor->start_time, monitor->coder_list[0].id);
			pthread_mutex_unlock(&(monitor->monitor_mutex));
			return (NULL);
		}
		pthread_cond_wait(&(monitor->monitor_cond), &(monitor->monitor_mutex));
	}
	pthread_cond_broadcast(&(monitor->monitor_cond));
	pthread_mutex_unlock(&(monitor->monitor_mutex));
	monitoring_loop(monitor);
	return (NULL);
}
