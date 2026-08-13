/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:51:26 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/13 10:54:44 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	fill_left_dongle_fifo(t_coder *coder, t_coder *left_coder)
{
	coder->left_dongle->queue[0] = left_coder->id;
	coder->left_dongle->queue[1] = coder->id;
}

void	fill_right_dongle_fifo(t_coder *coder, t_coder *right_coder)
{
	coder->right_dongle->queue[0] = right_coder->id;
	coder->right_dongle->queue[1] = coder->id;
}

void	fifo_queue(t_coder *coder)
{
	t_coder	*right_coder;
	t_coder	*left_coder;

	if (coder->id == 1)
		left_coder = &(
				coder->monitor->coder_list[coder->monitor->coders_nb - 1]);
	else
		left_coder = &(coder->monitor->coder_list[coder->id - 2]);
	if (coder->id == coder->monitor->coders_nb)
		right_coder = &(coder->monitor->coder_list[0]);
	else
		right_coder = &(coder->monitor->coder_list[coder->id]);
	fill_left_dongle_fifo(coder, left_coder);
	fill_right_dongle_fifo(coder, right_coder);
}
