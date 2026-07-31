/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:04:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/07/31 13:22:31 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_routine.h"

void	*coder_routine(t_coder coder)
{
	pthread_mutex_lock(coder.monitor->monitor_mutex);
	while (strcmp(coder.monitor->status, "READY") != 0)
	{
		printf("WAITING ROUTINE");
		pthread_cond_wait(coder.monitor->coder_cond, coder.mutex);
	}
	pthread_mutex_unlock(coder.monitor->monitor_mutex);
	printf("la\n");
}