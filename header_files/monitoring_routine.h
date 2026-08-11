#ifndef MONITORING_ROUTINE_H
# define MONITORING_ROUTINE_H 
# include "monitoring.h"
# include "sys/time.h"
# include <string.h>
# include <unistd.h>
# include "coder.h"

typedef struct coder t_coder;

void	*monitoring_routine(void* monitor);
void	print_log(char *string, t_coder *coder);
#endif