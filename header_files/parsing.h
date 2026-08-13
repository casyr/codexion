/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 16:13:48 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/13 16:16:05 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>

int	parsing(int argc, char **argv);
int	parsing_message(int argc, char **argv);
int	is_number_checker(char *argv);
int	scheduler_checker(char *argv);
#endif