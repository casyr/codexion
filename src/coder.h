#ifndef CODER_H
# define CODER_H

# include "monitoring.h"
# include "dongle.h"
# include "routine/coder_routine/coder_routine.h"
# include <pthread.h>

typedef struct monitor t_monitoring;

typedef struct coder
{
	char			*status;
	int				id;
	t_monitoring	*monitor;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
} t_coder;

int		coder_thread_join(int coders_nb, pthread_t *coder_th);
t_coder	*coder_list_init(int coders_nb, t_monitoring *monitor, t_dongle *dongle_list);

#endif
