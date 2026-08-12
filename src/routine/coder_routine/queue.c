/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 17:52:16 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/12 11:55:04 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	fill_left_dongle_queue(t_coder *coder, t_coder *left_coder, long middle_c_last_comp, long left_c_last_comp)
{
	if (middle_c_last_comp == left_c_last_comp)
	{
		if (coder->id < left_coder->id)
		{
			coder->left_dongle->queue[0] = left_coder->id;
			coder->left_dongle->queue[1] = coder->id;
		}
	}
	if (middle_c_last_comp < left_c_last_comp)
	{
		coder->left_dongle->queue[0] = left_coder->id;
		coder->left_dongle->queue[1] = coder->id;
		return ;
	}
	coder->left_dongle->queue[1] = left_coder->id;
	coder->left_dongle->queue[0] = coder->id;
}

void	fill_right_dongle_queue(t_coder *coder, t_coder *right_coder, long middle_c_last_comp, long right_c_last_comp)
{
	if (middle_c_last_comp == right_c_last_comp)
	{
		if (coder->id < right_coder->id)
		{
			coder->right_dongle->queue[0] = right_coder->id;
			coder->right_dongle->queue[1] = coder->id;
		}
	}
	if (middle_c_last_comp < right_c_last_comp)
	{
		coder->right_dongle->queue[0] = right_coder->id;
		coder->right_dongle->queue[1] = coder->id;
		return ;
	}
	coder->right_dongle->queue[1] = right_coder->id;
	coder->right_dongle->queue[0] = coder->id;
}


void	edf_queue(t_coder *coder)
{
	t_coder	*right_coder;
	t_coder	*left_coder;
	long		right_c_last_comp;
	long		left_c_last_comp;
	long		middle_c_last_comp;

	if (coder->id == 1)
		left_coder = &(coder->monitor->coder_list[coder->monitor->coders_nb - 1]);
	else
		left_coder = &(coder->monitor->coder_list[coder->id - 2]);

	if (coder->id == coder->monitor->coders_nb)
		right_coder = &(coder->monitor->coder_list[0]);
	else
		right_coder = &(coder->monitor->coder_list[coder->id]);

	right_c_last_comp = right_coder->last_compile;
	left_c_last_comp = left_coder->last_compile;
	middle_c_last_comp = coder->last_compile;

	fill_left_dongle_queue(coder, left_coder, middle_c_last_comp, left_c_last_comp);
	fill_right_dongle_queue(coder, right_coder, middle_c_last_comp, right_c_last_comp);
}

// int *fifo_queue()
// {
// 	int	queue[2];

// 	return (queue);
// }

void	scheduler_choose_and_update(t_coder *coder)
{
	if (strcmp(coder->monitor->scheduler, "edf") == 0)
	{
		edf_queue(coder);
	}
	// if (strcmp(coder->monitor->scheduler, "fifo") == 0)
	// {
	// 	fifo_queue();
	// }
}

int		is_schedule(t_coder *coder, t_dongle *first_dongle, t_dongle *second_dongle)
{
	if (coder->id == first_dongle->queue[1] && coder->id == second_dongle->queue[1])
		return(0);
	return (1);
}