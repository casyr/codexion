#include "coder.h"

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

t_coder	*coder_list_init(int coders_nb, t_monitoring *monitor)
{
	t_coder			*coder_list;
	pthread_mutex_t	coder_mutex;
	int				coder_index;

	coder_list = malloc(sizeof(t_coder) * coders_nb);
	if (!coder_list)
		return (NULL);
	coder_index = 0;
	while (coder_index < coders_nb)
	{
		if (pthread_mutex_init(&coder_mutex, NULL) != 0)
			return (NULL);
		coder_list[coder_index].id = coder_index + 1;
		coder_list[coder_index].status = "init";
		coder_list[coder_index].mutex = &coder_mutex;
		coder_list[coder_index].monitor = monitor;
		coder_index++;
	}
	return (coder_list);
}