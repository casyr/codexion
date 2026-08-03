/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/03 17:31:25 by yriffard         ###   ########.fr       */
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


t_monitoring	*monitoring_init(char **argv)
{
	pthread_mutex_t	monitor_mutex;
	pthread_cond_t	monitor_cond;
	pthread_mutex_t	coder_mutex;
	pthread_cond_t	coder_cond;
	t_monitoring *monitor = malloc(sizeof(t_monitoring));

    if (!monitor)
        return (NULL);

    monitor->monitor_mutex = malloc(sizeof(pthread_mutex_t));
    monitor->coder_mutex = malloc(sizeof(pthread_mutex_t));
    monitor->monitor_cond = malloc(sizeof(pthread_cond_t));
    monitor->coder_cond = malloc(sizeof(pthread_cond_t));

    if (!monitor->monitor_mutex || !monitor->coder_mutex || 
        !monitor->monitor_cond || !monitor->coder_cond)
        return (NULL);

    pthread_mutex_init(monitor->monitor_mutex, NULL);
    pthread_mutex_init(monitor->coder_mutex, NULL);
    pthread_cond_init(monitor->monitor_cond, NULL);
    pthread_cond_init(monitor->coder_cond, NULL);
	monitor->compiling_nb = atoi(argv[6]);
	monitor->last_compile = ft_get_time();
	if (monitor->last_compile == 1)
		return (NULL);
	monitor->time_to_burnout = atoi(argv[2]);
	monitor->coder_nb = atoi(argv[1]);
	monitor->status = "INIT";
	// printf("m cond: %p, c cond %p, coder m %p, monitor m %p", monitor->monitor_cond, monitor->coder_cond, monitor->coder_mutex, monitor->monitor_mutex);
	return (monitor);
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

int	coder_th_creation(t_coder *coder_list, int coders_nb, pthread_t *coder_th, t_monitoring *monitor)
{
	int				coder_index;
	pthread_cond_t	coder_cond;

	coder_index = 0;
	monitor->coder_cond = &coder_cond;
	while (coder_index < coders_nb)
	{
		printf("coder %i create\n", coder_index + 1);
		if (pthread_create(&(coder_th[coder_index]), NULL, &coder_routine, &(coder_list[coder_index])) != 0)
		{
			printf("coder %i thread CREATION fails\n", coder_index);
			return (1);
		}
		coder_list[coder_index].status = "READY";
		coder_index++;
	}
	return (0);
}
int		coder_are_ready(t_coder *coder_list, int coder_nb)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < coder_nb)
	{
		if (strcmp(coder_list[i].status, "READY") == 0)
			count += 1;
		i++;
	}
	
	// printf("%d / %d\n", count, coder_nb);
	if (count == coder_nb)
		return (1);
	return (0);
}

int	monitoring_th_creation(t_monitoring *monitor, pthread_t monitoring_th, t_coder *coder_list, int coders_nb)
{

	if (pthread_create(&monitoring_th, NULL, &monitoring_routine, monitor) != 0)
	{
		printf("monitoring thread CREATION fails");
		return (1);
	}
	while (coder_are_ready(coder_list, coders_nb) != 1)
	{
		pthread_mutex_lock(monitor->monitor_mutex);
		pthread_cond_wait(monitor->monitor_cond, monitor->monitor_mutex);
		printf("WAITING");
	}
	monitor->status = "READY";
	printf("READY\n");
	pthread_cond_broadcast(monitor->coder_cond);
	pthread_mutex_unlock(monitor->monitor_mutex);
	return (0);
}

t_coder	*coder_list_init(int coders_nb, t_monitoring *monitor)
{
	t_coder			*coder_list;
	pthread_mutex_t	coder_mutex;
	int				coder_index;

	coder_list = malloc(sizeof(t_coder) * coders_nb);
	if (!coder_list)
		return (NULL);
	coder_index = 0;
	while (coder_index < coders_nb)
	{
		if (pthread_mutex_init(&coder_mutex, NULL) != 0)
			return (NULL);
		coder_list[coder_index].id = coder_index + 1;
		coder_list[coder_index].status = "init";
		coder_list[coder_index].mutex = &coder_mutex;
		coder_list[coder_index].monitor = monitor;
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
		return (3);
	}
	monitor = monitoring_init(argv);
	if(!monitor)
	{
		printf("error alloc monitor");
		return (4);
	}
	coder_list = coder_list_init(coders_nb, monitor);
	if (!coder_list)
		return (5);
	if(coder_th_creation(coder_list, coders_nb, coder_th, monitor) != 0)
		return (6);
	if(monitoring_th_creation(monitor, monitoring_th, coder_list, coders_nb) != 0)
		return (7);
	coder_thread_join(coders_nb, coder_th);
	pthread_join(monitoring_th, NULL);
	pthread_cond_destroy(monitor->monitor_cond);
	pthread_cond_destroy(monitor->coder_cond);
	pthread_mutex_destroy(monitor->coder_mutex);
	free(coder_th);
	free(coder_list);
	free(monitor);

	return (0);
}

	// creer tous les dongles
	// finis l initialisation de codeur
	// 	// creer monitor

	// lance monitor + lance routine pour chaques codeurs
	// // join monitor + join routine pour chaques codeurs
	// free tout
