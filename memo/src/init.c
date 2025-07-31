/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:15:51 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 16:11:43 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief Initializes all mutexes required for the simulation.
 *
 * @param table A pointer to the main table structure.
 * @return Returns 0 on success, 1 on failure.
 * @note This includes a mutex for each fork, a mutex to protect printf,
 * and a mutex to protect access to meal-related data.
 */
static int	init_mutexes(t_table *table)
{
	int	i;

	i = 0;
	// Loop through each fork and initialize its corresponding mutex.
	while (i < table->num_philosophers)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (1); // Return 1 if initialization fails.
		i++;
	}
	// Initialize the mutex that prevents garbled output from printf.
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
		return (1);
	// Initialize the mutex that protects shared meal data like last_meal_time.
	if (pthread_mutex_init(&table->meal_lock, NULL) != 0)
		return (1);
	return (0);
}

/**
 * @brief Initializes the data for each philosopher.
 *
 * @param table A pointer to the main table structure.
 * @note Sets the ID, meal count, last meal time, and a pointer back to the
 * main table for each philosopher.
 */
static void	init_philosophers(t_table *table)
{
	int	i;

	i = 0;
	// Loop through and set initial values for each philosopher.
	while (i < table->num_philosophers)
	{
		table->philosophers[i].id = i + 1;
		table->philosophers[i].eat_count = 0;
		// All philosophers start as if they have just eaten to begin the timer.
		table->philosophers[i].last_meal_time = table->start_time;
		// Give each philosopher a pointer to the main table for easy access.
		table->philosophers[i].table = table;
		i++;
	}
}

/**
 * @brief Initializes the main table structure for the simulation.
 *
 * @param table A pointer to the table structure to be initialized.
 * @param ac The argument count from main.
 * @param av The argument vector from main.
 * @return Returns 0 on success, 1 on failure.
 * @note This function orchestrates the entire setup process.
 */
int	init_table(t_table *table, int ac, char **av)
{
	// 1. Parse command-line arguments and store them.
	table->num_philosophers = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		table->must_eat_count = ft_atoi(av[5]);
	else
		table->must_eat_count = -1; // -1 indicates this option is not used.
	// A philosopher count of 0 or less is invalid.
	if (table->num_philosophers <= 0)
		return (1);
	// 2. Allocate memory for the philosophers and forks.
	table->philosophers = malloc(sizeof(t_philosopher)
			* table->num_philosophers);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philosophers);
	// If memory allocation fails, return an error.
	if (!table->philosophers || !table->forks)
		return (1);
	// 3. Initialize all mutexes.
	if (init_mutexes(table) != 0)
		return (1);
	// 4. Set initial simulation state.
	table->simulation_should_end = 0;
	table->start_time = get_current_time();
	// 5. Initialize all philosopher structures.
	init_philosophers(table);
	return (0);
}
