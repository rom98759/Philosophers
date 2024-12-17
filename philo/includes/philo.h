/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:38:42 by rcaillie          #+#    #+#             */
/*   Updated: 2024/12/17 11:57:33 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t			thread;
	int					id;
	int					meals_eaten;
	size_t				last_meal;
	struct s_program	*program;
	pthread_mutex_t		*r_fork;
	pthread_mutex_t		*l_fork;
}						t_philo;

typedef struct s_program
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	size_t			start_time;
	int				dead_flag;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}					t_program;

/* Function Prototypes */
int		ft_parsing(int nb, char **av);
int		init_program(t_program *program, int ac, char **av);
int		ft_atoi(const char *str);
void	ft_error(void);
size_t	get_current_time(void);
void	ft_usleep(size_t milliseconds);

#endif
