/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 11:01:45 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/21 14:13:37 by yriffard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"
#include "monitoring.h"
#include "coder.h"

t_coder	*coder_list_init(int coders_nb, t_dongle *dongle_list,
	t_monitoring *monitor, t_coder *coder_list)
{
	int				coder_index;
	t_coder			*coder;

	coder_index = 0;
	while (coder_index < coders_nb)
	{
		coder = &(coder_list[coder_index]);
		coder->id = coder_index + 1;
		coder->compile_count = 0;
		coder->status = INIT;
		coder->right_dongle = &(dongle_list[coder_index]);
		coder->monitor = monitor;
		coder->last_compile = ft_get_time();
		if (coder_index == 0)
			coder->left_dongle = &(dongle_list[coders_nb - 1]);
		else
			coder->left_dongle = &(dongle_list[coder_index - 1]);
		coder_index++;
	}
	return (coder_list);
}

int	coder_th_creation(t_monitoring *monitor)
{
	int	coder_index;

	monitor->coder_th = malloc(monitor->coders_nb * sizeof(pthread_t));
	if (!monitor->coder_th)
		return (3);
	coder_index = 0;
	while (coder_index < monitor->coders_nb)
	{
		if (pthread_create(&(monitor->coder_th[coder_index]), NULL,
				&coder_routine, &(monitor->coder_list[coder_index])) != 0)
		{
			pthread_mutex_lock(&(monitor->monitor_mutex));
			monitor->status = FAIL;
			printf("coder %i thread CREATION fails\n", coder_index);
			pthread_cond_broadcast(&(monitor->monitor_cond));
			pthread_mutex_unlock(&(monitor->monitor_mutex));
			while (--coder_index >= 0)
				pthread_join(monitor->coder_th[coder_index], NULL);
			pthread_join(monitor->monitor_th, NULL);
			return (1);
		}
		coder_index++;
	}
	return (0);
}

void	*monitoring_init(char	**argv,
	long start_time, t_monitoring *monitor)
{
	monitor->start_time = start_time;
	monitor->dongle_list = NULL;
	monitor->coder_list = NULL;
	monitor->coder_th = NULL;
	monitor->coders_nb = atoi(argv[1]);
	monitor->time_to_burnout = atoi(argv[2]);
	monitor->time_to_compile = atoi(argv[3]);
	monitor->time_to_debug = atoi(argv[4]);
	monitor->time_to_refactor = atoi(argv[5]);
	monitor->compiling_nb = atoi(argv[6]);
	monitor->dongle_cooldown = atoi(argv[7]);
	monitor->scheduler = argv[8];
	monitor->status = INIT;
	monitor->finished_coders_nb = 0;
	monitor->total_compile_counter = 0;
	pthread_mutex_init(&(monitor->monitor_mutex), NULL);
	pthread_mutex_init(&(monitor->print_mutex), NULL);
	pthread_cond_init(&(monitor->monitor_cond), NULL);
	return (monitor);
}

void	dongle_list_init(int coders_nb, t_dongle *dongle_list)
{
	int				i;

	i = 0;
	while (i < coders_nb)
	{
		dongle_list[i].id = i + 1;
		dongle_list[i].is_free = true;
		pthread_mutex_init(&(dongle_list[i].dongle_mutex), NULL);
		dongle_list[i].last_release = ft_get_time();
		dongle_list[i].queue[0] = 0;
		dongle_list[i].queue[1] = 0;
		i++;
	}
}
