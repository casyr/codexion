/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/03 18:18:06 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"


void free_monitoring(void *monitor)
{
	free(monitor.coder_mutex);
	free(monitor.coder_cond);
	free(monitor.monitor_mutex);
	free(monitor.monitor_cond);
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
	monitor = monitoring_init(argv);
	if(!monitor)
	{
		printf("error alloc monitor");
		free_monitoring(monitor);
		return (4);
	}
	coder_list = coder_list_init(coders_nb, monitor);
	if (!coder_list)
		return (5);
		free_monitoring(monitor);
	if(coder_th_creation(coder_list, coders_nb, coder_th, monitor) != 0)
		return (6);
		free_monitoring(monitor);

	if(monitoring_th_creation(monitor, monitoring_th, coder_list, coders_nb) != 0)
		return (7);
		free_monitoring(monitor);

	coder_thread_join(coders_nb, coder_th);
	pthread_join(monitoring_th, NULL);
	pthread_cond_destroy(monitor->monitor_cond);
	pthread_cond_destroy(monitor->coder_cond);
	pthread_mutex_destroy(monitor->coder_mutex);
	free(coder_th);
	free(coder_list);
	free_monitoring(monitor);

	return (0);
}

	// creer tous les dongles
	// finis l initialisation de codeur
	// 	// creer monitor

	// lance monitor + lance routine pour chaques codeurs
	// // join monitor + join routine pour chaques codeurs
	// free tout
