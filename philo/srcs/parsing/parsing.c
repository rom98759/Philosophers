/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:52:20 by rcaillie          #+#    #+#             */
/*   Updated: 2024/12/12 10:53:08 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_valid(long n)
{
	if (n <= 0 || n > 2147483647)
		return (0);
	return (1);
}

int	is_digit(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_args(char *s)
{
	long	n;

	if (!s)
		return (0);
	n = ft_atol(s);
	if (!is_digit(s) || !is_valid(n))
		return (1);
	return (0);
}

int	ft_parsing(int nb, char **av)
{
	int	i;

	i = 1;
	while (i < nb)
	{
		if (check_args(av[i]))
		{
			ft_error();
			return (0);
		}
		else
			printf("%s\n", av[i]);
		i++;
	}
	return (1);
}
