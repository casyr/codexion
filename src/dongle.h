#ifndef DONGLE_H
# define DONGLE_H
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>


typedef struct	s_dongle
{
	int				is_free;
	int				id;
	pthread_mutex_t	*dongle_mutex;
} t_dongle;

t_dongle	*dongle_list_init(int coders_nb);

#endif
