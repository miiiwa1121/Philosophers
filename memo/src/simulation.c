/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:16:08 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 14:55:20 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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
