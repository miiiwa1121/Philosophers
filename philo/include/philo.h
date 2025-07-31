/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:15:44 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 13:41:02 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_table	t_table;

typedef struct s_philosopher
{
	int					id;
	long				last_meal_time;
	int					eat_count;
	pthread_t			thread;
	t_table				*table;
}	t_philosopher;

typedef struct s_table
{
	int					num_philosophers;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	int					must_eat_count;
	long				start_time;
	t_philosopher		*philosophers;
	pthread_mutex_t		*forks;
	int					simulation_should_end;
	pthread_mutex_t		print_lock;
	pthread_mutex_t		meal_lock;
}	t_table;

/* init.c */
int		init_table(t_table *table, int ac, char **av);

/* main.c */
void	cleanup(t_table *table);

/* simulation.c */
int		start_simulation(t_table *table);
void	*philosopher_routine(void *arg);
void	monitor_philosophers(t_table *table);
void	eat_action(t_philosopher *philo);

/* utils.c */
long	ft_atoi(const char *str);
long	get_current_time(void);
void	print_status(t_philosopher *philo, const char *status);

#endif