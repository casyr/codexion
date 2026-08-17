/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:48:57 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/17 18:54:39 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

static int has_priority(t_coder *a, t_coder *b)
{
	if (a->last_compile != b->last_compile)
		return (a->last_compile < b->last_compile);
	if (a->compile_count != b->compile_count)
		return (a->compile_count < b->compile_count);
	return (a->id < b->id);
}

void fill_right_dongle(t_coder *coder, t_coder *right_coder)
{
	if (has_priority(coder, right_coder))
	{
		coder->right_dongle->queue[1] = coder->id;
		coder->right_dongle->queue[0] = right_coder->id;
	}
	else
	{
		coder->right_dongle->queue[1] = right_coder->id;
		coder->right_dongle->queue[0] = coder->id;
	}
}

void fill_left_dongle(t_coder *coder, t_coder *left_coder)
{
	if (has_priority(coder, left_coder))
	{
		coder->left_dongle->queue[1] = coder->id;
		coder->left_dongle->queue[0] = left_coder->id;
	}
	else
	{
		coder->left_dongle->queue[1] = left_coder->id;
		coder->left_dongle->queue[0] = coder->id;
	}
}

void	edf_queue(t_coder *coder)
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
	fill_right_dongle(coder, right_coder);
	fill_left_dongle(coder, left_coder);
}
