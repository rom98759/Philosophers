/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:05:10 by rcaillie          #+#    #+#             */
/*   Updated: 2025/02/26 10:56:25 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Check if a philosopher has died
 */
static int	check_philo_death(t_program *program, int i)
{
	size_t	current_time;

	current_time = get_current_time();
	pthread_mutex_lock(&program->write_lock);
	if ((current_time - program->philos[i].last_meal)
		> (size_t)program->time_to_die)
	{
		pthread_mutex_lock(&program->dead_lock);
		program->dead_flag = 1;
		printf("%zu %d died\n", current_time - program->start_time,
			program->philos[i].id);
		pthread_mutex_unlock(&program->dead_lock);
		pthread_mutex_unlock(&program->write_lock);
		return (1);
	}
	pthread_mutex_unlock(&program->write_lock);
	return (0);
}

/**
 * Check if all philosophers have eaten
 */
static int	check_all_philos_full(t_program *program)
{
	int	i;
	int	all_ate;

	i = 0;
	all_ate = 1;
	while (i < program->nb_philos)
	{
		pthread_mutex_lock(&program->write_lock);
		if (program->philos[i].meals_eaten < program->max_meals
			|| program->max_meals == -1)
			all_ate = 0;
		pthread_mutex_unlock(&program->write_lock);
		i++;
	}
	return (all_ate);
}

/**
 * Routine master thread
 */
void	*game_master(void *arg)
{
	t_program	*program;
	int			i;

	program = (t_program *)arg;
	while (1)
	{
		i = -1;
		while (++i < program->nb_philos)
			if (check_philo_death(program, i))
				return (NULL);
		if (check_all_philos_full(program))
		{
			pthread_mutex_lock(&program->write_lock);
			pthread_mutex_lock(&program->dead_lock);
			program->dead_flag = 1;
			printf("All philosophers have eaten the maximum number of meals\n");
			pthread_mutex_unlock(&program->dead_lock);
			pthread_mutex_unlock(&program->write_lock);
			break ;
		}
		usleep(1);
	}
	return (NULL);
}
