/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:54:55 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 14:55:29 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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