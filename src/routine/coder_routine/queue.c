/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 17:52:16 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/20 15:35:33 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

int	equality_case(t_coder *other, t_coder *coder, int other_id)
{
	if (other->compile_count < coder->compile_count)
		return (0);
	if (other->compile_count == coder->compile_count
		&& other_id < coder->id)
		return (0);
	return (1);
}

int	edf_queue(t_dongle *dongle, t_coder *coder)
{
	long	coder_deadline;
	long	other_deadline;
	int		other_id;
	t_coder	*other;

	other_id = dongle->queue[1];
	if (other_id == coder->id)
		other_id = dongle->queue[0];
	if (other_id <= 0 || other_id == coder->id)
		return (1);
	other = &coder->monitor->coder_list[other_id - 1];
	if (strcmp(other->status, "FINISH") == 0)
		return (1);
	coder_deadline = coder->last_compile + coder->monitor->time_to_burnout;
	other_deadline = other->last_compile + other->monitor->time_to_burnout;
	if (other_deadline < coder_deadline)
		return (0);
	if (other_deadline == coder_deadline)
		return (equality_case(other, coder, other_id));
	return (1);
}

int	fifo_queue(t_dongle *dongle, t_coder *coder)
{
	return (dongle->queue[0] == coder->id);
}

int	are_schedule(t_coder *coder, t_dongle *first_dongle,
	t_dongle *second_dongle)
{
	if (strcmp(coder->monitor->scheduler, "edf") == 0)
	{
		if (edf_queue(first_dongle, coder) && edf_queue(second_dongle, coder))
			return (1);
		return (0);
	}
	return (fifo_queue(first_dongle, coder)
		&& fifo_queue(second_dongle, coder));
}
