/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:38:05 by rcaillie          #+#    #+#             */
/*   Updated: 2025/01/28 14:07:52 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	end_simulation(t_program *program)
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

int	main(int ac, char **av)
{
	t_program	program;
	pthread_t	master;
	int			i;

	if (!(ac == 5 || ac == 6))
	{
		ft_error();
		return (1);
	}
	if (!ft_parsing(ac, av))
		return (1);
	if (init_program(&program, ac, av))
		return (1);
	i = -1;
	while (++i < program.nb_philos)
	{
		if (pthread_create(&program.philos[i].thread, NULL,
			philo_routine, &program.philos[i]) != 0)
		{
			program.dead_flag = 1;
			break ;
		}
		if (program.nb_philos > 1)
			usleep(3);
	}
	if (i == program.nb_philos && pthread_create(&master, NULL, game_master, &program) != 0)
		program.dead_flag = 1;
	i = -1;
	while (++i < program.nb_philos)
		pthread_join(program.philos[i].thread, NULL);
	pthread_join(master, NULL);
	end_simulation(&program);
	return (0);
}
