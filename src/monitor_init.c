#include "monitoring.h"

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

int monitor_thread_join(pthread_t monitoring_th)
{
	if (pthread_join(monitoring_th, NULL) != 0)
	{
		printf("monitoring thread JOIN fails\n");
		return (1);
	}
	return (0);
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
	return (monitor);
}