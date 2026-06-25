/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/06/25 16:03:58 by yriffard         ###   ########.fr       */
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



int	monitoring_init(t_monitoring *monitor, pthread_mutex_t *monitor_mutex,
						char **argv, t_coder *coder_list)
{
	monitor->compiling_nb = atoi(argv[6]);
	monitor->last_compile = ft_get_time();
	monitor->mutex = monitor_mutex;
	if (monitor->last_compile == 1)
	{
		printf("gettimeofday of last_compile fail");
		return (1);
	}
	monitor->time_to_burnout = atoi(argv[2]);
	return (0);
}

// void coder_init(t_coder *coder)
// {
	
// }

int	coder_thread_creation(t_coder *coder_list, int coders_nb, pthread_t *coder_th)
{
	t_coder		coder;
	int			coder_index;
 
	coder_index = 0;
	while (coder_index < coders_nb)
	{
		coder.id = coder_index;
		coder.state = "NOT FINISH";
		if(pthread_create(&(coder_th[coder_index]), NULL, &routine, &coder) != 0)
		{
			printf("coder %i thread CREATION fails\n", coder_index);
			return (1);
		}
		coder_list[coder_index].id = coder_index;
		coder_list[coder_index].state = coder.state;
		coder_index++;
	}
	return (0);
}

int	monitoring_thread_creation(t_monitoring monitor, pthread_t *monitoring_th)
{
	if (pthread_create(monitoring_th, NULL, &monitoring_routine, &monitor) != 0)
	{
		printf("monitoring thread CREATION fails");
		return (1);
	}
	return (0);
}

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

int		main(int argc, char **argv)
{
	int 			coders_nb;
	int				compiling_nb;
	pthread_t 		*coder_th;
	pthread_t		monitoring_th;
	pthread_mutex_t	monitor_mutex;
	t_monitoring	monitor;
	t_coder			coder;
	t_coder			*coder_list;

	if (parsing(argc, argv) != 0)
		return (-1);
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
		return (1);
	}
	coder_th = malloc(coders_nb * sizeof(pthread_t));
	coder_list = malloc(sizeof(coder) * coders_nb);
	if (!coder_th || !coder_list)
	{
		printf("malloc fails");
		return (1);
	}
	if(coder_thread_creation(coder_list, coders_nb, coder_th) != 0)
		return (1);
	if (pthread_mutex_init(&monitor_mutex, NULL) !=0)
		return (2);
	if(monitoring_init(&monitor, &monitor_mutex, argv, coder_list) != 0)
		return (3);
	if(monitoring_thread_creation(monitor, &monitoring_th) != 0)
		return (4);
	if (coder_thread_join(coders_nb, coder_th) != 0)
		return (5);
	if (monitor_thread_join(monitoring_th) != 0)
		return (6);
	if (pthread_mutex_destroy(&monitor_mutex) != 0)
		return (7);
	return (0);
}
