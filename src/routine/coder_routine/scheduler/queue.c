/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 17:52:16 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/13 19:06:07 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	scheduler_choose_and_update(t_coder *coder)
{
	if (strcmp(coder->monitor->scheduler, "edf") == 0)
		edf_queue(coder);
	if (strcmp(coder->monitor->scheduler, "fifo") == 0)
		fifo_queue(coder);
}

int	is_schedule(t_coder *coder, t_dongle *first_dongle, t_dongle *second_dongle)
{
	if ((coder->id == 
			first_dongle->queue[1]) && (coder->id == second_dongle->queue[1]))
		return (0);
	return (1);
}
