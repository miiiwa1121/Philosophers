/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:15:56 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 05:21:13 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_numeric(char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

static int	validate_arguments(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo <num_of_philos> <time_to_die> <time_to_eat> \
<time_to_sleep> [num_of_times_each_philo_must_eat]\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		if (!is_numeric(argv[i]))
		{
			printf("Error: All arguments must be positive integers.\n");
			return (1);
		}
		i++;
	}
	return (0);
}

void	cleanup(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->print_lock);
	pthread_mutex_destroy(&table->meal_lock);
	free(table->forks);
	free(table->philosophers);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (validate_arguments(ac, av) != 0)
		return (1);
	if (init_table(&table, ac, av) != 0)
	{
		printf("Error: Initialization failed.\n");
		return (1);
	}
	if (start_simulation(&table) != 0)
	{
		cleanup(&table);
		printf("Error: Failed to start simulation.\n");
		return (1);
	}
	cleanup(&table);
	return (0);
}