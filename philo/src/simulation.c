/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:16:08 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 05:21:40 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat_action(t_philosopher *philo)
{
	t_table	*table;
	int		left_fork_id;
	int		right_fork_id;

	table = philo->table;
	left_fork_id = philo->id - 1;
	right_fork_id = philo->id % table->num_philosophers;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&table->forks[left_fork_id]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&table->forks[right_fork_id]);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&table->forks[right_fork_id]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&table->forks[left_fork_id]);
		print_status(philo, "has taken a fork");
	}
	print_status(philo, "is eating");
	pthread_mutex_lock(&table->meal_lock);
	philo->last_meal_time = get_current_time();
	philo->eat_count++;
	pthread_mutex_unlock(&table->meal_lock);
	usleep(table->time_to_eat * 1000);
	pthread_mutex_unlock(&table->forks[left_fork_id]);
	pthread_mutex_unlock(&table->forks[right_fork_id]);
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

void	monitor_philosophers(t_table *table)
{
	int	i;
	int	all_philos_have_eaten_enough;

	while (1)
	{
		i = 0;
		all_philos_have_eaten_enough = 1;
		while (i < table->num_philosophers)
		{
			pthread_mutex_lock(&table->meal_lock);
			if (get_current_time() - table->philosophers[i].last_meal_time
				> table->time_to_die)
				print_status(&table->philosophers[i], "died");
			if (table->must_eat_count != -1
				&& table->philosophers[i].eat_count < table->must_eat_count)
				all_philos_have_eaten_enough = 0;
			pthread_mutex_unlock(&table->meal_lock);
			i++;
		}
		if (table->must_eat_count != -1 && all_philos_have_eaten_enough)
		{
			pthread_mutex_lock(&table->print_lock);
			table->simulation_should_end = 1;
			pthread_mutex_unlock(&table->print_lock);
		}
		pthread_mutex_lock(&table->print_lock);
		if (table->simulation_should_end)
		{
			pthread_mutex_unlock(&table->print_lock);
			break ;
		}
		pthread_mutex_unlock(&table->print_lock);
		usleep(1000);
	}
}

int	start_simulation(t_table *table)
{
	int	i;

	if (table->num_philosophers == 1)
	{
		table->start_time = get_current_time();
		print_status(&table->philosophers[0], "has taken a fork");
		usleep(table->time_to_die * 1000);
		print_status(&table->philosophers[0], "died");
		return (0);
	}
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
