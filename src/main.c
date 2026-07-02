/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/07/02 12:01:26 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

long	ft_get_time()
{
	struct timeval	current_time;
	long 			result;

	if(gettimeofday(&current_time, NULL) != 0)
		return (1);
	result = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (result);
}


int	monitoring_init(t_monitoring *monitor, char **argv, t_coder *coder_list)
{
	pthread_mutex_t	monitor_mutex;
	pthread_cond_t	monitor_cond;
	pthread_mutex_t	coder_mutex;
	pthread_cond_t	coder_cond;

	if (pthread_mutex_init(&coder_mutex, NULL) != 0)
		return(1);
	if (pthread_mutex_init(&monitor_mutex, NULL) !=0)
		return (2);
	if (pthread_cond_init(&coder_cond, NULL) != 0)
		return (3);
	if (pthread_cond_init(&monitor_cond, NULL) != 0)
		return (4);
	monitor->compiling_nb = atoi(argv[6]);
	monitor->last_compile = ft_get_time();
	if (monitor->last_compile == 1)
		return (3);
	monitor->time_to_burnout = atoi(argv[2]);
	monitor->coder_nb = atoi(argv[1]);
	monitor->status = "INIT";
	monitor->monitor_cond = malloc(sizeof(pthread_cond_t));
	if (!monitor->monitor_cond)
		return (5);
	monitor->monitor_cond = &monitor_cond;

	monitor->coder_cond = malloc(sizeof(pthread_cond_t));
	if (!monitor->coder_cond)
		return (6);
	monitor->coder_cond = &coder_cond;

	monitor->coder_mutex =  malloc(sizeof(pthread_mutex_t));
	if (!monitor->coder_mutex)
		return (7);	
	monitor->coder_mutex = &coder_mutex;

	monitor->monitor_mutex = malloc(sizeof(pthread_mutex_t));
	if (!monitor->monitor_mutex)
		return (8);	
	monitor->monitor_mutex = &monitor_mutex;
	return (0);
}

// void coder_init(t_coder *coder)
// {
	
// }

int	coder_thread_join(int coders_nb, pthread_t *coder_th)
{
	int coder_index;

	coder_index = 0;
	while (coder_index < coders_nb)
	{
		if (pthread_join(coder_th[coder_index], NULL) != 0)
		{
			printf("coder %d thread JOIN fails\n", coder_index);
			return (1);
		}
		coder_index++;
	}
	return(0);
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

int	coder_th_creation_and_join(t_coder *coder_list, int coders_nb, pthread_t *coder_th, t_monitoring *monitor)
{
	int				coder_index;
	pthread_mutex_t	coder_mutex;
	pthread_cond_t	coder_cond;
 
	coder_index = 0;
	pthread_cond_init(&coder_cond, NULL);
	monitor->coder_cond = &coder_cond;
	while (coder_index < coders_nb - 1)
	{
		if (pthread_create(&(coder_th[coder_index]), NULL, &coder_routine, (void*)(coder_list[coder_index]) != 0))
		{
			printf("coder %i thread CREATION fails\n", coder_index);
			return (1);
		}
		// coder_list[coder_index]->status = "READY";
	}
	while (strcmp(monitor->status, "READY") != 0)
		pthread_cond_wait(&coder_cond, &coder_mutex);
	coder_thread_join(coders_nb, coder_th);
	pthread_mutex_destroy(&coder_mutex);
	pthread_cond_destroy(&coder_cond);
	return (0);
}
int		coder_are_ready(t_coder *coder_list, int coder_nb)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < coder_nb - 1)
	{
		if (strcmp(coder_list[i].status, "ready") == 0)
			count += 1;
		i++;
	}
	if (count == coder_nb)
		return (1);
	return (0);
}

int	monitoring_th_creation_and_join(t_monitoring *monitor, pthread_t *monitoring_th, t_coder *coder_list, int coders_nb)
{
	if (pthread_create(monitoring_th, NULL, &monitoring_routine, monitor) != 0)
	{
		printf("monitoring thread CREATION fails");
		return (1);
	}
	while (coder_are_ready(coder_list, coders_nb) != 1)
		pthread_cond_wait(monitor->monitor_cond, monitor->monitor_mutex);
	pthread_cond_broadcast(monitor->coder_cond);
	monitor_thread_join(*monitoring_th);
	pthread_cond_destroy(monitor->monitor_cond);
	pthread_cond_destroy(monitor->coder_cond);
	pthread_mutex_destroy(monitor->coder_mutex);
	return (0);
}

t_coder	*coder_list_init(int coders_nb)
{
	t_coder			*coder_list;
	pthread_mutex_t	coder_mutex;
	int				coder_index;

	coder_list = malloc(sizeof(t_coder) * coders_nb);
	if (!coder_list)
		return (NULL);
	coder_index = 0;
	while (coder_index < coders_nb - 1)
	{
		if (pthread_mutex_init(&coder_mutex, NULL) != 0)
			return (NULL);
		coder_list[coder_index].id = coder_index;
		coder_list[coder_index].status = "init";
		coder_list[coder_index].mutex = &coder_mutex;
		coder_index++;
	}
	return (coder_list);
}
 
int		main(int argc, char **argv)
{
	int 			coders_nb;
	int				compiling_nb;
	pthread_t 		*coder_th;
	pthread_t		monitoring_th;

	t_monitoring	monitor;
	t_coder			coder;
	t_coder			*coder_list;

	parsing_message(argc, argv);
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
	coder_list = coder_list_init(coders_nb);
	if (!coder_th || !coder_list)
	{
		printf("malloc fails");
		return (3);
	}
	if(monitoring_init(&monitor, argv, coder_list) != 0)
		return (4);
	if(coder_th_creation_and_join(coder_list, coders_nb, coder_th, &monitor) != 0)
		return (6);
	if(monitoring_th_creation_and_join(&monitor, &monitoring_th, coder_list, coders_nb) != 0)
		return (5);
	return (0);
}

	// creer tous les dongles
	// finis l initialisation de codeur
	// 	// creer monitor

	// lance monitor + lance routine pour chaques codeurs
	// // join monitor + join routine pour chaques codeurs
	// free tout
