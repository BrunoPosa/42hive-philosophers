/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:29:44 by bposa             #+#    #+#             */
/*   Updated: 2024/08/16 00:05:51 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	my_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

static int	my_isdigit(int c)
{
	if (c >= 48 && c <= 57)
	{
		return (1);
	}
	return (0);
}

static int	aredigits(char **s)
{
	int	i;

	i = 0;
	while (*(++s))
	{
		i = 0;
		if ((*s)[i] == '\0')
			return (0);
		while ((*s)[i])
		{
			if (my_isdigit((*s)[i++]) == 0)
				return (0);
		}
	}
	return (1);
}

static int	islenok(char **s)
{
	if (my_strlen(s[1]) > 3
		|| my_strlen(s[2]) > 6
		|| my_strlen(s[3]) > 6
		|| my_strlen(s[4]) > 6)
		return (0);
	if (s[5])
	{
		if (my_strlen(s[5]) > 6)
			return (0);
	}
	return (1);
}

int	validator(int argc, char **args)
{
	if (argc < 5 || argc > 6)
		return (ermsg(EARGC));
	if (aredigits(args) != 1 || islenok(args) != 1)
		return (ermsg(EARG));
	if (my_atoi(args[1]) == 0 || my_atoi(args[1]) > 400 || my_atoi(args[2]) == 0
		|| my_atoi(args[3]) == 0 || my_atoi(args[4]) == 0)
		return (ermsg(EARG));
	if (args[5])
	{
		if (my_atoi(args[5]) == 0)
			return (ermsg(EARG));
	}
	return (1);
}
