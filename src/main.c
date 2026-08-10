/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/10 11:45:59 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void free_all(t_monitoring *monitor, t_dongle *dongle_list, pthread_t *coder_th, t_coder *coder_list)
{
	int	i;

	i = 0;
	free(monitor->monitor_mutex);
	free(monitor->monitor_cond);
	free(monitor->print_mutex);
	free(monitor);

	while (i < monitor->coders_nb)
	{
		free(dongle_list[i].dongle_mutex);
		i++;
	}
	free(dongle_list);

	free(coder_th);
	// while (i < monitor->coders_nb)
	// {
	// 	free(coder_list[i].left_dongle);
	// 	free(coder_list[i].right_dongle);
	// 	i++;
	// }
	free(coder_list);
}

void destroy_all(t_monitoring *monitor)
{
	pthread_cond_destroy(monitor->monitor_cond);
	pthread_mutex_destroy(monitor->monitor_mutex);
	pthread_mutex_destroy(monitor->print_mutex);
}
 
int	main(int argc, char **argv)
{
	int 			coders_nb;
	int				compiling_nb;
	pthread_t 		*coder_th;
	pthread_t		monitoring_th;
	t_monitoring	*monitor;
	t_coder			coder;
	t_coder			*coder_list;
	t_dongle		*dongle_list;
	long			start_time;

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
	if(!dongle_list)
	{
		printf("error malloc dongle_list");
		// free_all(monitor, dongle_list);
		return (4);
	}
	monitor = monitoring_init(argv, dongle_list, start_time);
	if(!monitor)
	{
		printf("error malloc monitor");
		// free_all(monitor, dongle_list);
		return (4);
	}
	
	coder_list = coder_list_init(coders_nb, dongle_list, monitor);
	if (!coder_list)
	{
		return (5);
		// free_all(monitor, dongle_list); /// 
	}
	
	monitor->coder_list = coder_list;

	if(monitoring_th_creation(monitor, &monitoring_th, coder_list, coders_nb) != 0)
	{
		return (6);
		// free_all(monitor, dongle_list); /// 
	}

	if(coder_th_creation(coder_list, coders_nb, coder_th, monitor) != 0)
	{
		return (7);
		free_all(monitor, dongle_list, coder_th, coder_list);
	}


	coder_thread_join(coders_nb, coder_th);
	if (pthread_join(monitoring_th, NULL) != 0)
	{
		return (8);
		free_all(monitor, dongle_list, coder_th, coder_list);
	}
	destroy_all(monitor);
	free_all(monitor, dongle_list, coder_th, coder_list);

	return (0);
}

