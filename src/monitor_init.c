/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 09:05:08 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/06 09:01:07 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitoring.h"

int	monitoring_th_creation(t_monitoring *monitor, pthread_t *monitoring_th, t_coder *coder_list, int coders_nb)
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

t_monitoring	*monitoring_init(char **argv, t_coder *coder_list, t_dongle *dongle_list, long start_time)
{
	pthread_mutex_t	monitor_mutex;
	pthread_mutex_t	print_mutex;
	pthread_cond_t	monitor_cond;
	t_monitoring *monitor = malloc(sizeof(t_monitoring));

    if (!monitor)
        return (NULL);

    monitor->monitor_mutex = malloc(sizeof(pthread_mutex_t));
    monitor->monitor_cond = malloc(sizeof(pthread_cond_t));
	monitor->print_mutex = malloc(sizeof(pthread_mutex_t));

    if (!monitor->monitor_mutex || !monitor->monitor_cond )
        return (NULL);

    pthread_mutex_init(monitor->monitor_mutex, NULL);
    pthread_mutex_init(monitor->print_mutex, NULL);
    pthread_cond_init(monitor->monitor_cond, NULL);

	monitor->last_compile = ft_get_time();
	monitor->last_dongle_release = ft_get_time();
	if (monitor->last_compile == 1)
		return (NULL);

	monitor->coder_list = coder_list;
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
	monitor->scheduler = argv[7];
	return (monitor);
}
