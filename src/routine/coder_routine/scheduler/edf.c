/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 13:48:57 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/13 10:54:18 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	fill_left_dongle_edf(t_coder *coder, t_coder *left_coder,
	long middle_c_last_comp, long left_c_last_comp)
{
	if (middle_c_last_comp == left_c_last_comp)
	{
		if (coder->id < left_coder->id)
		{
			coder->left_dongle->queue[0] = left_coder->id;
			coder->left_dongle->queue[1] = coder->id;
			return ;
		}
		coder->left_dongle->queue[1] = left_coder->id;
		coder->left_dongle->queue[0] = coder->id;
		return ;
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

void	fill_right_dongle_edf(t_coder *coder, t_coder *right_coder,
	long middle_c_last_comp, long right_c_last_comp)
{
	if (middle_c_last_comp == right_c_last_comp)
	{
		if (coder->id < right_coder->id)
		{
			coder->right_dongle->queue[0] = right_coder->id;
			coder->right_dongle->queue[1] = coder->id;
			return ;
		}
		coder->right_dongle->queue[1] = right_coder->id;
		coder->right_dongle->queue[0] = coder->id;
		return ;
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
	t_coder		*right_coder;
	t_coder		*left_coder;
	long		right_c_last_comp;
	long		left_c_last_comp;
	long		middle_c_last_comp;

	if (coder->id == 1)
		left_coder = &(
				coder->monitor->coder_list[coder->monitor->coders_nb - 1]);
	else
		left_coder = &(coder->monitor->coder_list[coder->id - 2]);
	if (coder->id == coder->monitor->coders_nb)
		right_coder = &(coder->monitor->coder_list[0]);
	else
		right_coder = &(coder->monitor->coder_list[coder->id]);
	right_c_last_comp = right_coder->last_compile;
	left_c_last_comp = left_coder->last_compile;
	middle_c_last_comp = coder->last_compile;
	fill_left_dongle_edf(coder, left_coder, middle_c_last_comp,
		left_c_last_comp);
	fill_right_dongle_edf(coder, right_coder, middle_c_last_comp,
		right_c_last_comp);
}
