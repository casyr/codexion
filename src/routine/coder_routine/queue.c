/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 17:52:16 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/11 18:03:15 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

int *edf_queue()
{
	
}

int *fifo_queue()
{
	int	queue[2];

	return (queue);
}

int schedule_chooser(t_coder *coder)
{
	if (strcmp(coder->monitor->scheduler, "edf") == 0)
	{
		coder->left_dongle->queue = edf_queue();
	}
	if (strcmp(coder->monitor->scheduler, "fifo") == 0)
	{
		coder->left_dongle->queue = fifo_queue();
	}
}

int is_schedule(t_coder *coder)
{
	if (coder->id == coder->left_dongle->queue[2] && coder->id == coder->right_dongle->queue[2])
		return(0);
	return (1);
}