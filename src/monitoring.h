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
	pthread_mutex_t	*mutex;
} t_monitoring;

#endif