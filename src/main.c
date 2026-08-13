/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/13 18:39:28 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	inits_fails(t_monitoring *monitor)
{
	printf("error malloc fail");
	destroy_all(monitor);
	free_all(monitor);
	return (1);
}

int	init_structs(t_monitoring *monitor, int coders_nb, long start_time, char **argv)
{
	t_coder			*coder_list;
	t_dongle		*dongle_list;
	pthread_t		*coder_th;

	coder_list = NULL;
	coder_th = NULL;
	dongle_list = NULL;
	monitoring_init(argv, dongle_list, start_time, monitor);
	if (!monitor)
		return (inits_fails(monitor));
	dongle_list = dongle_list_init(coders_nb);
	if (!dongle_list)
		return (inits_fails(monitor));
	monitor->dongle_list = dongle_list;
	coder_list = coder_list_init(coders_nb, dongle_list, monitor);
	if (!coder_list)
		return (inits_fails(monitor));
	monitor->coder_list = coder_list;
	return (0);
}

int	threads_creation(t_monitoring *monitor, int coders_nb)
{
	if (monitoring_th_creation(monitor) != 0)
	{
		printf("error monitor thread");
		destroy_all(monitor);
		free_all(monitor);
		return (1);
	}
	if (coder_th_creation(monitor->coder_list,
			coders_nb, monitor->coder_th, monitor) != 0)
	{
		printf("error coder thread");
		destroy_all(monitor);
		free_all(monitor);
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
			destroy_all(monitor);
			free_all(monitor);
			printf("coder %d thread JOIN fails\n", coder_index);
			return (1);
		}
		coder_index++;
	}
	if (pthread_join(monitor->monitor_th, NULL) != 0)
	{
		printf("error coder thread join");
		destroy_all(monitor);
		free_all(monitor);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int				coders_nb;
	int				compiling_nb;
	long			start_time;
	t_monitoring	*monitor;

	monitor = malloc(sizeof(t_monitoring));
	start_time = ft_get_time();
	if (parsing_message(argc, argv) != 0)
		return (1);
	coders_nb = atoi(argv[1]);
	compiling_nb = atoi(argv[6]);
	if (init_structs(monitor, coders_nb, start_time, argv))
		return (1);
	if (threads_creation(monitor, coders_nb))
		return (1);
	if (threads_join(monitor))
		return (1);
	destroy_all(monitor);
	free_all(monitor);
	return (0);
}
// if (coders_nb < 2)
// {
// 	printf("number of coders must be > 1");
// 	return (1);
// }
// if (compiling_nb == 0)
// {
// 	printf("compiling number must be > 0");
// 	return (2);
// }