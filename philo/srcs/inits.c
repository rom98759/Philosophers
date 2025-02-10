/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 20:58:57 by rcaillie          #+#    #+#             */
/*   Updated: 2025/02/10 10:45:40 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Initialize philosopher structures.
 */
static void	init_philo(t_program *program)
{
	int	i;

	i = -1;
	while (++i < program->nb_philos)
	{
		program->philos[i].id = i + 1;
		program->philos[i].meals_eaten = 0;
		program->philos[i].last_meal = program->start_time;
		program->philos[i].r_fork = &program->forks[i];
		program->philos[i].l_fork = &program
			->forks[(i + 1) % program->nb_philos];
		program->philos[i].program = program;
	}
}

/**
 * Parse command-line arguments.
 */
static int	parse_arguments(t_program *program, int ac, char **av)
{
	program->nb_philos = ft_atol(av[1]);
	program->time_to_die = ft_atol(av[2]);
	program->time_to_eat = ft_atol(av[3]);
	program->time_to_sleep = ft_atol(av[4]);
	if (ac == 6)
		program->max_meals = ft_atol(av[5]);
	else
		program->max_meals = -1;
	program->dead_flag = 0;
	program->start_time = get_current_time();
	return (0);
}

/**
 * Allocate resources for forks and philosophers.
 */
static int	allocate_resources(t_program *program)
{
	program->forks = malloc(sizeof(pthread_mutex_t) * program->nb_philos);
	if (!program->forks)
	{
		ft_error();
		return (1);
	}
	program->philos = malloc(sizeof(t_philo) * program->nb_philos);
	if (!program->philos)
	{
		free(program->forks);
		ft_error();
		return (1);
	}
	return (0);
}

/**
 * Initialize mutexes for forks and program locks.
 */
static int	init_mutexes(t_program *program)
{
	int	i;

	if (pthread_mutex_init(&program->write_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&program->dead_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&program->write_lock);
		return (1);
	}
	i = -1;
	while (++i < program->nb_philos)
	{
		if (pthread_mutex_init(&program->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&program->forks[i]);
			pthread_mutex_destroy(&program->write_lock);
			pthread_mutex_destroy(&program->dead_lock);
			return (1);
		}
	}
	return (0);
}

/**
 * Initialize the program by parsing arguments, allocating resources,
 * initializing mutexes, and setting up philosophers.
 */
int	init_program(t_program *program, int ac, char **av)
{
	if (parse_arguments(program, ac, av) || allocate_resources(program))
		return (1);
	if (init_mutexes(program))
	{
		free(program->philos);
		free(program->forks);
		return (1);
	}
	init_philo(program);
	return (0);
}
