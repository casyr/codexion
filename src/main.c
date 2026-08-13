/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/13 12:08:25 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void free_all(t_monitoring *monitor,
	t_dongle *dongle_list, pthread_t *coder_th, t_coder *coder_list, int coders_nb)
{
	int	i;

	i = 0;
	if (monitor)
	{
		if (monitor->monitor_mutex)
			free(monitor->monitor_mutex);
		if (monitor->monitor_cond)
			free(monitor->monitor_cond);
		if (monitor->print_mutex)
			free(monitor->print_mutex);
		free(monitor);
	}
	if (dongle_list)
	{
		while (i < coders_nb)
		{
			if (dongle_list[i].dongle_mutex)
				free(dongle_list[i].dongle_mutex);
			i++;
		}
		free(dongle_list);
	}
	if (coder_list)
		free(coder_list);

	if (coder_th)
		free(coder_th);
}

void	destroy_all(t_monitoring *monitor, t_dongle *dongle_list, int coders_nb)
{
	int	i;

	i = 0;
	if (monitor)
	{
		if (monitor->monitor_cond)
			pthread_cond_destroy(monitor->monitor_cond);
		if (monitor->monitor_mutex)
			pthread_mutex_destroy(monitor->monitor_mutex);
		if (monitor->print_mutex)
			pthread_mutex_destroy(monitor->print_mutex);
	}
	while (i < coders_nb)
	{
		if (!dongle_list)
			break ;
		if (dongle_list[i].dongle_mutex)
			pthread_mutex_destroy(dongle_list[i].dongle_mutex);
		i++;
	}
}
 
int	main(int argc, char **argv)
{
	int 			coders_nb;
	int				compiling_nb;
	pthread_t 		*coder_th;
	pthread_t		monitoring_th;
	t_monitoring	*monitor;
	t_coder			*coder_list;
	t_dongle		*dongle_list;
	long			start_time;

	monitor = NULL;
	coder_list = NULL;
	start_time = ft_get_time();
	if (parsing_message(argc, argv) != 0)
		return (1);
	coders_nb = atoi(argv[1]);
	compiling_nb = atoi(argv[6]);
	if (coders_nb < 2)
	{
		printf("number of coders must be > 1");
		return (1);
	}
	if (compiling_nb == 0)
	{
		printf("compiling number must be > 0");
		return (2);
	}
	coder_th = malloc(coders_nb * sizeof(pthread_t));
	if (!coder_th)
	{
		printf("malloc fails");
		return (3);
	}
	dongle_list = dongle_list_init(coders_nb);
	if (!dongle_list)
	{
		printf("error malloc dongle_list");
		destroy_all(monitor, dongle_list, coders_nb);
		free_all(monitor, dongle_list, coder_th, coder_list, coders_nb);
		return (4);
	}
	monitor = monitoring_init(argv, dongle_list, start_time);
	if (!monitor)
	{
		printf("error monitor init");
		destroy_all(monitor, dongle_list, coders_nb);
		free_all(monitor, dongle_list, coder_th, coder_list, coders_nb);
		return (4);
	}
	coder_list = coder_list_init(coders_nb, dongle_list, monitor);
	if (!coder_list)
	{
		printf("error coder list init");
		destroy_all(monitor, dongle_list, coders_nb);
		free_all(monitor, dongle_list, coder_th, coder_list, coders_nb);
		return (5);
	}
	monitor->coder_list = coder_list;
	if (monitoring_th_creation(monitor, &monitoring_th) != 0)
	{
		printf("error monitor thread");
		destroy_all(monitor, dongle_list, coders_nb);
		free_all(monitor, dongle_list, coder_th, coder_list, coders_nb);
		return (6);
	}
	if (coder_th_creation(coder_list, coders_nb, coder_th, monitor) != 0)
	{
		printf("error coder thread");
		destroy_all(monitor, dongle_list, coders_nb);
		free_all(monitor, dongle_list, coder_th, coder_list, coders_nb);
		return (7);
	}
	coder_thread_join(coders_nb, coder_th);
	if (pthread_join(monitoring_th, NULL) != 0)
	{
		printf("error coder thread join");
		free_all(monitor, dongle_list, coder_th, coder_list, coders_nb);
		return (8);
	}
	destroy_all(monitor, dongle_list, coders_nb);
	free_all(monitor, dongle_list, coder_th, coder_list, coders_nb);
	return (0);
}
