/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:16:08 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 14:42:21 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/*
** =============================================================================
** Helper Functions for Monitor
** =============================================================================
*/

static int	check_death(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->table->meal_lock);
	if (get_current_time() - philo->last_meal_time > philo->table->time_to_die)
	{
		print_status(philo, "died");
		pthread_mutex_unlock(&philo->table->meal_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->meal_lock);
	return (0);
}

static int	check_all_eaten(t_table *table)
{
	int	i;
	int	all_eaten;

	if (table->must_eat_count == -1)
		return (0);
	all_eaten = 1;
	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_mutex_lock(&table->meal_lock);
		if (table->philosophers[i].eat_count < table->must_eat_count)
			all_eaten = 0;
		pthread_mutex_unlock(&table->meal_lock);
		i++;
	}
	if (all_eaten)
	{
		pthread_mutex_lock(&table->print_lock);
		table->simulation_should_end = 1;
		pthread_mutex_unlock(&table->print_lock);
		return (1);
	}
	return (0);
}

/*
** =============================================================================
** Philosopher's Actions & Main Routine
** =============================================================================
*/

static void	take_forks(t_philosopher *philo)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id - 1;
	right_fork_id = philo->id % philo->table->num_philosophers;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->table->forks[left_fork_id]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->table->forks[right_fork_id]);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->table->forks[right_fork_id]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->table->forks[left_fork_id]);
		print_status(philo, "has taken a fork");
	}
}

static void	drop_forks(t_philosopher *philo)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id - 1;
	right_fork_id = philo->id % philo->table->num_philosophers;
	pthread_mutex_unlock(&philo->table->forks[left_fork_id]);
	pthread_mutex_unlock(&philo->table->forks[right_fork_id]);
}

void	eat_action(t_philosopher *philo)
{
	take_forks(philo);
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->table->meal_lock);
	philo->last_meal_time = get_current_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->table->meal_lock);
	usleep(philo->table->time_to_eat * 1000);
	drop_forks(philo);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_table			*table;

	philo = (t_philosopher *)arg;
	table = philo->table;
	if (philo->id % 2 != 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&table->print_lock);
		if (table->simulation_should_end)
		{
			pthread_mutex_unlock(&table->print_lock);
			break ;
		}
		pthread_mutex_unlock(&table->print_lock);
		eat_action(philo);
		print_status(philo, "is sleeping");
		usleep(table->time_to_sleep * 1000);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

/*
** =============================================================================
** Simulation Control
** =============================================================================
*/

void	monitor_philosophers(t_table *table)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < table->num_philosophers)
		{
			if (check_death(&table->philosophers[i]))
				return ;
			i++;
		}
		if (check_all_eaten(table))
			return ;
		usleep(1000);
	}
}

static int	create_and_join_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
	{
		if (pthread_create(&table->philosophers[i].thread, NULL,
				&philosopher_routine, &table->philosophers[i]) != 0)
			return (1);
		i++;
	}
	monitor_philosophers(table);
	i = 0;
	while (i < table->num_philosophers)
	{
		if (pthread_join(table->philosophers[i].thread, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	start_simulation(t_table *table)
{
	if (table->num_philosophers == 1)
	{
		table->start_time = get_current_time();
		print_status(&table->philosophers[0], "has taken a fork");
		usleep(table->time_to_die * 1000);
		print_status(&table->philosophers[0], "died");
		return (0);
	}
	return (create_and_join_threads(table));
}