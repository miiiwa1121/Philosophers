/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:54:41 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 15:07:47 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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
