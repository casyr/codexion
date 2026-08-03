#ifndef MONITORING_H
# define MONITORING_H

# include "coder.h"
# include "routine/monitoring_routine/monitoring_routine.h"
# include <pthread.h>

typedef struct coder t_coder;

typedef struct monitor
{
	long			last_compile;
	long			time_to_burnout;
	int				compiling_nb;
	t_coder 		*coder_list;
	int				coder_nb;

	char			*status;
	pthread_mutex_t	*monitor_mutex;
	pthread_cond_t	*monitor_cond;
	pthread_mutex_t	*coder_mutex;
	pthread_cond_t	*coder_cond;
} t_monitoring;

int	monitoring_th_creation(t_monitoring *monitor, pthread_t monitoring_th, t_coder *coder_list, int coders_nb);
int	coder_th_creation(t_coder *coder_list, int coders_nb, pthread_t *coder_th, t_monitoring *monitor);
int monitor_thread_join(pthread_t monitoring_th);
t_monitoring	*monitoring_init(char **argv);



#endif