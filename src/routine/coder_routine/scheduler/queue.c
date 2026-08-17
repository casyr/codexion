/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 17:52:16 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/17 18:13:32 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	scheduler_choose_and_update(t_coder *coder)
{
	pthread_mutex_lock(coder->monitor->monitor_mutex);
	if (strcmp(coder->monitor->scheduler, "edf") == 0)
	{
		edf_queue(coder);
		pthread_mutex_unlock(coder->monitor->monitor_mutex);
		return ;
	}
	fifo_queue(coder);
	pthread_mutex_unlock(coder->monitor->monitor_mutex);
}

int	is_schedule(t_coder *coder, t_dongle *first_dongle, t_dongle *second_dongle)
{
	if ((first_dongle->queue[1] == coder->id)
		&& (second_dongle->queue[1] == coder->id))
		return (0);
	return (1);
}
