#include "coder.h"

typedef struct monitor
{
	int		last_compile;
	int		time_to_burnout;
	int		compiling_nb;
	t_coder *coder_list;
} t_monitoring;