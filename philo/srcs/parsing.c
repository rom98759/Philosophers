/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:52:20 by rcaillie          #+#    #+#             */
/*   Updated: 2025/02/10 10:48:22 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Check if a number is valid (greater than 0 and less than or equal to INT_MAX).
 */
static int	is_valid(long n)
{
	if (n <= 0 || n > 2147483647)
		return (0);
	return (1);
}

/**
 * Check if a string contains only digits.
 */
static int	is_digit(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/**
 * Check if a command-line argument is valid.
 */
static int	check_arg(const char *s)
{
	int	n;

	if (!is_digit(s))
		return (0);
	n = ft_atol(s);
	if (!is_valid(n))
		return (0);
	return (1);
}

/**
 * Parse and validate command-line arguments.
 */
int	ft_parsing(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (!check_arg(av[i]))
		{
			ft_error();
			return (0);
		}
		i++;
	}
	return (1);
}
