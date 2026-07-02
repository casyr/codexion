#ifndef MONITORING_H
# define MONITORING_H

# include "coder.h"
# include "routine/monitoring_routine/monitoring_routine.h"
# include <pthread.h>

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

#endif