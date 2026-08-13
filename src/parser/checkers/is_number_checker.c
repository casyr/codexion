/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_number_checker.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 10:05:25 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/13 10:22:44 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_number_checker(char *argv)
{
	int	i;
	int	c;

	i = 0;
	while (argv[i])
	{
		c = argv[i];
		if (argv[i] < '0' || argv[i] > '9')
			return (1);
		i++;
	}
	return (0);
}
