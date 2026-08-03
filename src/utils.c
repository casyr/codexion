#include "coder.h"

long	ft_get_time()
{
	struct timeval	current_time;
	long 			result;

	if(gettimeofday(&current_time, NULL) != 0)
		return (1);
	result = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (result);
}


int		coder_are_ready(t_coder *coder_list, int coder_nb)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < coder_nb)
	{
		if (strcmp(coder_list[i].status, "READY") == 0)
			count += 1;
		i++;
	}
	
	// printf("%d / %d\n", count, coder_nb);
	if (count == coder_nb)
		return (1);
	return (0);
}