/* ************************************************************************** */

/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:04:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/03 13:27:52 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_routine.h"

void	*coder_routine(void *v_coder)
{
	t_coder	coder;

	coder = *(t_coder*) v_coder;
	printf("coder %i start his routine and wait\n", coder.id);
	coder = *(t_coder*) v_coder;
	pthread_mutex_lock(coder.monitor->coder_mutex);
	while (strcmp(coder.monitor->status, "READY") != 0)
	{
		// printf("WAITING ROUTINE");
		pthread_cond_wait(coder.monitor->coder_cond, coder.mutex);
	}
	printf("coder %i start finish waiting\n", coder.id);
	pthread_mutex_unlock(coder.monitor->coder_mutex);
	// printf("la\n");
	////
}