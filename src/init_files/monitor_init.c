/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 09:05:08 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/19 14:56:54 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header_files/monitoring.h"

int	monitoring_th_creation(t_monitoring *monitor)
{
	if (pthread_create(&monitor->monitor_th, NULL,
			&monitoring_routine, monitor) != 0)
	{
		printf("monitoring thread CREATION fails");
		return (1);
	}
	return (0);
}

int	monitor_thread_join(pthread_t monitoring_th)
{
	if (pthread_join(monitoring_th, NULL) != 0)
	{
		printf("monitoring thread JOIN fails\n");
		return (1);
	}
	return (0);
}

void	*monitoring_init(char	**argv, t_dongle *dongle_list,
	long start_time, t_monitoring *monitor)
{
	monitor->total_compile_counter = 0;

	monitor->last_compile = ft_get_time();
	monitor->last_dongle_release = ft_get_time();
	monitor->start_time = start_time;
	monitor->status = "INIT";
	monitor->dongle_list = dongle_list;
	monitor->coders_nb = atoi(argv[1]);
	monitor->time_to_burnout = atoi(argv[2]);
	monitor->time_to_compile = atoi(argv[3]);
	monitor->time_to_debug = atoi(argv[4]);
	monitor->time_to_refactor = atoi(argv[5]);
	monitor->compiling_nb = atoi(argv[6]);
	monitor->dongle_cooldown = atoi(argv[7]);
	monitor->scheduler = argv[8];
	monitor->finished_coders_nb = 0;
	pthread_mutex_init(&(monitor->monitor_mutex), NULL);
	pthread_mutex_init(&(monitor->print_mutex), NULL);
	pthread_cond_init(&(monitor->monitor_cond), NULL);
	return (monitor);
}
