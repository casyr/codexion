/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_rountine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:02:13 by yriffard          #+#    #+#             */
/*   Updated: 2026/06/25 16:03:12 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitoring_routine.h"

void	*monitoring_routine(void* monitor)
{
	int 			compiling_counter;
	long 			time;
	struct timeval	current_time;
	t_monitoring 	monitoring;
	
	monitoring = *(t_monitoring*)monitor;
	while(1)
	{
		time = ft_get_time();
		if (time == 1)
		{
			printf("gettimeofday fail in monitoring rountine");
			break;
		}
		pthread_mutex_lock(monitoring.mutex);
		// printf("%ld - %ld = %ld > %d \n", time, monitoring.last_compile, time - monitoring.last_compile, monitoring.time_to_burnout);
		if (time - monitoring.last_compile > monitoring.time_to_burnout)
		{
			printf("BURNOUT!");
			break;
		}
		pthread_mutex_unlock(monitoring.mutex);
	}
}