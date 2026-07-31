#ifndef CODER_H
# define CODER_H

# include "monitoring.h"
# include "routine/coder_routine/coder_routine.h"
# include <pthread.h>

typedef struct monitor t_monitoring;

typedef struct coder
{
	char			*status;
	int				id;
	pthread_mutex_t	*mutex;
	t_monitoring	*monitor;
} t_coder;

#endif