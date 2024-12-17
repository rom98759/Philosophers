/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 20:58:57 by rcaillie          #+#    #+#             */
/*   Updated: 2024/12/17 20:58:57 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philo(t_program *program)
{
	int	i;

	i = -1;
	while (++i < program->nb_philos)
		pthread_mutex_init(&program->forks[i], NULL);
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

static int	parse_arguments(t_program *program, int ac, char **av)
{
	program->nb_philos = atoi(av[1]);
	program->time_to_die = atoi(av[2]);
	program->time_to_eat = atoi(av[3]);
	program->time_to_sleep = atoi(av[4]);
	if (ac == 6)
		program->max_meals = atoi(av[5]);
	else
		program->max_meals = -1;
	program->dead_flag = 0;
	program->start_time = get_current_time();
	return (0);
}

static int	allocate_resources(t_program *program)
{
	program->forks = malloc(sizeof(pthread_mutex_t) * program->nb_philos);
	program->philos = malloc(sizeof(t_philo) * program->nb_philos);
	if (!program->forks || !program->philos)
		return (ft_error(), 1);
	return (0);
}

static int	init_mutexes(t_program *program)
{
	int	i;

	pthread_mutex_init(&program->write_lock, NULL);
	pthread_mutex_init(&program->dead_lock, NULL);
	i = -1;
	while (++i < program->nb_philos)
		pthread_mutex_init(&program->forks[i], NULL);
	return (0);
}

int	init_program(t_program *program, int ac, char **av)
{
	if (parse_arguments(program, ac, av) || allocate_resources(program))
		return (1);
	init_mutexes(program);
	init_philo(program);
	return (0);
}
