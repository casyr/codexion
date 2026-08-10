/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:02:13 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/10 12:02:31 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitoring_routine.h"

void	*monitoring_routine(void* monitoring)
{
	int 			compiling_counter;
	long 			time;
	struct timeval	current_time; 
	t_monitoring 	*monitor;
	int				i;

	i = 0;
	monitor = (t_monitoring*)monitoring;
	pthread_mutex_lock(monitor->monitor_mutex);
	while (strcmp(monitor->status, "READY") != 0)
		pthread_cond_wait(monitor->monitor_cond, monitor->monitor_mutex);
	pthread_cond_broadcast(monitor->monitor_cond);
	pthread_mutex_unlock(monitor->monitor_mutex);
	while(1)
	{
		time = ft_get_time();
		if (time == 1)
		{
			printf("gettimeofday fail in monitor rountine");
			break;
		}
		// printf("%ld - %ld = %ld > %ld \n", time, monitor->last_compile, time - monitor->last_compile, monitor->time_to_burnout);
		pthread_mutex_lock(monitor->monitor_mutex);
		while(i < monitor->coders_nb)
		{
			if (time - monitor->coder_list[i].last_compile > monitor->time_to_burnout)
			{
				monitor->status = "BURNOUT";
				pthread_mutex_unlock(monitor->monitor_mutex);
				break;
			}
			i++;
		}
		if (strcmp(monitor->status, "BURNOUT") == 0)
			break;
		pthread_mutex_unlock(monitor->monitor_mutex);
		usleep(1000);
	}
	return (NULL);
}
