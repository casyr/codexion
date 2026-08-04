/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:04:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/04 15:56:17 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_routine.h"

void	*coder_routine(void *v_coder)
{
	t_coder	*coder;

	coder = (t_coder*)v_coder;
	coder->status = "READY";
	printf("coder %i start his routine and wait\n", coder->id);
	pthread_mutex_lock(coder->monitor->monitor_mutex);
	while (strcmp(coder->monitor->status, "READY") != 0)
	{
		// printf("WAITING ROUTINE");
		pthread_cond_wait(coder->monitor->monitor_cond, coder->monitor->monitor_mutex);
	}
	printf("coder %i finish waiting\n", coder->id);
	pthread_mutex_unlock(coder->monitor->monitor_mutex);
	printf("finish routine\n");
	// printf("la\n");
	////
}
