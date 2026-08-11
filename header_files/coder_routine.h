#ifndef CODER_ROUTINE_H
# define CODER_ROUTINE_H
# include "coder.h"
# include "monitoring.h"
# include "stdio.h"

typedef struct monitor t_monitoring;

long	ft_get_time();
void	*coder_routine(void *v_coder);
void 	print_log(char *string, t_coder *coder);
void	ft_usleep(long time_in_ms, t_monitoring *monitor);
int		is_schedule(t_coder *coder);
#endif