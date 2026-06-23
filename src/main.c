/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/06/23 11:02:01 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int parsing(int argc, char **argv)
{
	int	parsed_data;

	parsed_data = parser(argc, argv);

	if (parsed_data == 1)
		printf("the number of args isn't correct.");
	if (parsed_data == 2)
		printf("numbers of coder didn't respect the format.");
	if (parsed_data == 3)
		printf("time to burnout didn't respect the format.");
	if (parsed_data == 4)
		printf("time to compile didn't respect the format.");
	if (parsed_data == 5)
		printf("time to debug didn't respect the format.");
	if (parsed_data == 6)
		printf("time to refactor didn't respect the format.");
	if (parsed_data == 7)
		printf("number of compiles required didn't respect the format.");
	if (parsed_data == 8)
		printf("dongle cooldown didn't respect the format.");
	if (parsed_data == 9)
		printf("sheduler didn't respect the format.");
	return (parsed_data);
}

int	main(int argc, char **argv)
{
	int parsed_data;

	parsed_data = parsing(argc, argv);
	if (parsed_data != 0)
		return parsed_data;
}
