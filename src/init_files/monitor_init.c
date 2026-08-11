/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 09:05:08 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/11 14:00:39 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header_files/monitoring.h"

int	monitoring_th_creation(t_monitoring *monitor, pthread_t *monitoring_th)
{

	if (pthread_create(monitoring_th, NULL, &monitoring_routine, monitor) != 0)
	{
		printf("monitoring thread CREATION fails");
		return (1);
	}
	return (0);
}

int monitor_thread_join(pthread_t monitoring_th)
{
	if (pthread_join(monitoring_th, NULL) != 0)
	{
		printf("monitoring thread JOIN fails\n");
		return (1);
	}
	return (0);
}

t_monitoring	*monitoring_init(char **argv, t_dongle *dongle_list, long start_time)
{
	t_monitoring *monitor; 

	monitor = malloc(sizeof(t_monitoring));
	if (!monitor) /////////// corriger par !monitor
		return (NULL);

	monitor->monitor_mutex = malloc(sizeof(pthread_mutex_t));
	if (!monitor->monitor_mutex )
	{
		free(monitor);
		return (NULL);
	}

	monitor->monitor_cond = malloc(sizeof(pthread_cond_t));
	if (!monitor->monitor_cond)
	{
		free(monitor->monitor_mutex);
		free(monitor);
		return (NULL);
	}

	monitor->finished_coders_nb = 0;

	monitor->print_mutex = malloc(sizeof(pthread_mutex_t));
	if (!monitor->print_mutex)
	{
		free(monitor->monitor_mutex);
		free(monitor->monitor_cond);
		free(monitor);
		return (NULL);
	}

	pthread_mutex_init(monitor->monitor_mutex, NULL);
	pthread_mutex_init(monitor->print_mutex, NULL);
	pthread_cond_init(monitor->monitor_cond, NULL);

	monitor->total_compile_counter = 0;

	monitor->last_compile = ft_get_time();
	monitor->last_dongle_release = ft_get_time();
	if (monitor->last_compile == 1)
		return (NULL);

	monitor->status = "INIT";
	monitor->dongle_list = dongle_list;
	monitor->start_time = start_time;
	monitor->coders_nb = atoi(argv[1]);
	monitor->time_to_burnout = atoi(argv[2]);
	monitor->time_to_compile = atoi(argv[3]);
	monitor->time_to_debug = atoi(argv[4]);
	monitor->time_to_refactor = atoi(argv[5]);
	monitor->compiling_nb = atoi(argv[6]);
	monitor->dongle_cooldown = atoi(argv[7]);
	monitor->scheduler = argv[8];
	return (monitor);
}
