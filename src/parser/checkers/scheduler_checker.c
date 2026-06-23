/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_checker.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 12:07:30 by yriffard          #+#    #+#             */
/*   Updated: 2026/06/23 12:40:46 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

int	scheduler_checker(char *argv)

{
	if (strcmp(argv, "fifo") == 0 || strcmp(argv, "edf") == 0)
		return (0);
	return (1);
}
