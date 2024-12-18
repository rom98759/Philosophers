/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:38:05 by rcaillie          #+#    #+#             */
/*   Updated: 2024/12/18 10:23:38 by rcaillie         ###   ########.fr       */
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
}

int	main(int ac, char **av)
{
	t_program	program;
	pthread_t	master;
	int			i;

	if (!(ac == 5 || ac == 6))
		return (ft_error(), 1);
	if (!ft_parsing(ac, av))
		return (1);
	if (init_program(&program, ac, av))
		return (1);
	i = -1;
	while (++i < program.nb_philos)
	{
		pthread_create(&program.philos[i].thread, NULL,
			philo_routine, &program.philos[i]);
		if (program.nb_philos > 1)
			usleep(400);
	}
	pthread_create(&master, NULL, game_master, &program);
	pthread_join(master, NULL);
	i = -1;
	while (++i < program.nb_philos)
		pthread_join(program.philos[i].thread, NULL);
	end_simulation(&program);
	return (0);
}
