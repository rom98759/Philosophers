/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:38:05 by rcaillie          #+#    #+#             */
/*   Updated: 2025/02/10 10:48:31 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * End the simulation by destroying mutexes and freeing resources.
 */
static void	end_simulation(t_program *program)
{
	int	i;

	i = -1;
	while (++i < program->nb_philos)
	{
		pthread_mutex_destroy(&program->forks[i]);
	}
	pthread_mutex_destroy(&program->write_lock);
	pthread_mutex_destroy(&program->dead_lock);
	free(program->forks);
	free(program->philos);
	program->forks = NULL;
	program->philos = NULL;
}

/**
 * Create threads for each philosopher.
 */
static int	create_philos_threads(t_program *program)
{
	int	i;

	i = -1;
	while (++i < program->nb_philos)
	{
		if (pthread_create(&program->philos[i].thread, NULL,
				philo_routine, &program->philos[i]) != 0)
		{
			program->dead_flag = 1;
			return (1);
		}
		if (program->nb_philos > 1)
			usleep(3);
	}
	return (0);
}

/**
 * Create the master thread to monitor the simulation.
 */
static int	create_master_thread(t_program *program, pthread_t *master)
{
	if (pthread_create(master, NULL, game_master, program) != 0)
	{
		program->dead_flag = 1;
		return (1);
	}
	return (0);
}

/**
 * Wait for all threads to finish.
 */
static void	wait_threads(t_program *program, pthread_t *master)
{
	int	i;

	i = -1;
	while (++i < program->nb_philos)
		pthread_join(program->philos[i].thread, NULL);
	pthread_join(*master, NULL);
}

/**
 * Main function to start the simulation.
 */
int	main(int ac, char **av)
{
	t_program	program;
	pthread_t	master;

	if (!(ac == 5 || ac == 6))
	{
		ft_error();
		return (1);
	}
	if (!ft_parsing(ac, av))
		return (1);
	if (init_program(&program, ac, av))
		return (1);
	if (create_philos_threads(&program))
		return (1);
	if (create_master_thread(&program, &master))
		return (1);
	wait_threads(&program, &master);
	end_simulation(&program);
	return (0);
}
