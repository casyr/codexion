/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/04 17:03:09 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void free_monitoring(t_monitoring *monitor)
{
	free(monitor->monitor_mutex);
	free(monitor->monitor_cond);
	free(monitor);
}
 
int		main(int argc, char **argv)
{
	int 			coders_nb;
	int				compiling_nb;
	pthread_t 		*coder_th;
	pthread_t		monitoring_th;

	t_monitoring	*monitor;
	t_coder			coder;
	t_coder			*coder_list;

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
		free(coder_th);
		return (3);
	}
	monitor = monitoring_init(argv, coder_list);
	if(!monitor)
	{
		printf("error alloc monitor");
		free_monitoring(monitor);
		return (4);
	}
	coder_list = coder_list_init(coders_nb, monitor);
	if (!coder_list)
	{
		return (5);
		free_monitoring(monitor);
	}
	monitor->coder_list = coder_list;
	if(monitoring_th_creation(monitor, &monitoring_th, coder_list, coders_nb) != 0)
	{
		return (6);
		free_monitoring(monitor);
	}
	if(coder_th_creation(coder_list, coders_nb, coder_th, monitor) != 0)
	{
		return (7);
		free_monitoring(monitor);
	}
	coder_thread_join(coders_nb, coder_th);
	if (pthread_join(monitoring_th, NULL) != 0)
	{
		return (8);
		free_monitoring(monitor);
	}
	pthread_cond_destroy(monitor->monitor_cond);
	free(coder_th);
	free_monitoring(monitor);
	free(coder_list);
	
	return (0);
}

	// creer tous les dongles
	// finis l initialisation de codeur
	// 	// creer monitor

	// lance monitor + lance routine pour chaques codeurs
	// // join monitor + join routine pour chaques codeurs
	// free tout
