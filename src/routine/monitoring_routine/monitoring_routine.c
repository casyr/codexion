/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:02:13 by yriffard          #+#    #+#             */
/*   Updated: 2026/06/26 14:41:55 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitoring_routine.h"

int		coder_are_ready(t_monitoring monitor)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < monitor.coder_nb - 1)
	{
		if (strcmp(monitor.coder_list[i].status, "ready") == 0)
			count += 1;
	}
	if (count == monitor.coder_nb)
		return (1);
	return (0);
}

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
		while (coder_are_ready(monitoring) == 0)
		{
			pthread_cond_wait(monitoring.monitor_cond, monitoring.mutex);
		}
		monitoring.status = "ready";
		pthread_cond_broadcast(monitoring.coder_cond);
		pthread_mutex_unlock(monitoring.mutex);
	}
	return (monitoring.mutex);
}


thread cree -> est ce que ton id est N ?
si non -> condwait
else ->  coders_are_ready to ok + 
