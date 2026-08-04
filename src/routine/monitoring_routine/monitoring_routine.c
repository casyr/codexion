/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:02:13 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/04 17:46:04 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitoring_routine.h"


void	*monitoring_routine(void* monitoring)
{
	int 			compiling_counter;
	long 			time;
	struct timeval	current_time; 
	t_monitoring 	*monitor;

	printf("monitoring thread is created\n");
	monitor = (t_monitoring*)monitoring;
	pthread_mutex_lock(monitor->monitor_mutex);

	while (strcmp(monitor->status, "READY") != 0)
	{	
		// printf("CAR: %d\n", coder_are_ready(monitor->coder_list, monitor->coders_nb));

		printf("monitor is waiting\n");
		pthread_cond_wait(monitor->monitor_cond, monitor->monitor_mutex);
		printf("monitor finish waiting\n");
	}
	monitor->status = "READY";
	pthread_cond_broadcast(monitor->monitor_cond);
	while(1)
	{
		time = ft_get_time();
		if (time == 1)
		{
			printf("gettimeofday fail in monitor rountine");
			pthread_mutex_unlock(monitor->monitor_mutex);
			break;
		}
		// printf("\n%p\n",  monitoring->monitor_mutex);
		// printf("%ld - %ld = %ld > %d \n", time, monitoring.last_compile, time - monitoring.last_compile, monitoring.time_to_burnout);
		if (time - monitor->last_compile > monitor->time_to_burnout)
		{
			printf("BURNOUT!");
			break;
		}
		pthread_mutex_unlock(monitor->monitor_mutex);
		usleep(1000);
	}
	return (NULL);
}
