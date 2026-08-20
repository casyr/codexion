/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/20 21:56:41 by yriffard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	init_structs(t_monitoring *monitor, long start_time, char **argv)
{
	int				coders_nb;

	coders_nb = atoi(argv[1]);
	monitoring_init(argv, start_time, monitor);
	monitor->dongle_list = malloc(sizeof(t_dongle) * coders_nb);
	if (!monitor->dongle_list)
	{
		printf("dongle list malloc fail\n");
		return (1);
	}
	dongle_list_init(coders_nb, monitor->dongle_list);
	monitor->coder_list = malloc(sizeof(t_coder) * coders_nb);
	if (!monitor->coder_list)
	{
		printf("coder list malloc fail\n");
		return (1);
	}
	coder_list_init(coders_nb, monitor->dongle_list, monitor,
		monitor->coder_list);
	return (0);
}

int	threads_creation(t_monitoring *monitor)
{
	if (pthread_create(&monitor->monitor_th, NULL,
			&monitoring_routine, monitor) != 0)
	{
		printf("error monitor thread\n");
		return (1);
	}
	if (coder_th_creation(monitor) != 0)
	{
		printf("error coder thread\n");
		return (1);
	}
	return (0);
}

int	threads_join(t_monitoring *monitor)
{
	int	coder_index;

	coder_index = 0;
	while (coder_index < monitor->coders_nb)
	{
		if (pthread_join(monitor->coder_th[coder_index], NULL) != 0)
		{
			printf("coder %d thread JOIN fails\n", coder_index);
			return (1);
		}
		coder_index++;
	}
	if (pthread_join(monitor->monitor_th, NULL) != 0)
	{
		printf("error coder thread join\n");
		return (1);
	}
	return (0);
}

void	destroy_and_free(t_monitoring *monitor)
{
	destroy_all(monitor);
	free_all(monitor);
}

int	main(int argc, char **argv)
{
	long			start_time;
	t_monitoring	monitor;

	start_time = ft_get_time();
	if (parsing_message(argc, argv) != 0)
		return (1);
	if (init_structs(&monitor, start_time, argv))
	{
		destroy_and_free(&monitor);
		return (1);
	}
	if (threads_creation(&monitor))
	{
		destroy_and_free(&monitor);
		return (2);
	}
	if (threads_join(&monitor))
	{
		destroy_and_free(&monitor);
		return (3);
	}
	destroy_and_free(&monitor);
	return (0);
}
