#ifndef CODER_H
# define CODER_H

# include "routine/coder_routine/coder_routine.h"
# include <pthread.h>

typedef struct coder
{
	char			*status;
	int				id;
	pthread_mutex_t	*mutex;
} t_coder;

#endif