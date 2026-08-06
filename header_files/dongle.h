#ifndef DONGLE_H
# define DONGLE_H
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include "coder.h"

typedef struct coder t_coder;

typedef struct	s_dongle
{
	int				is_free;
	int				id;
	pthread_mutex_t	*dongle_mutex;
	int				queue[2];
} t_dongle;

t_dongle	*dongle_list_init(int coders_nb);

#endif
