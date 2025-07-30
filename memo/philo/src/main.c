/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:15:56 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 05:15:57 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void cleanup(t_table *table)
{
    int i;

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

int is_numeric(char *str)
{
    while (*str)
    {
        if (*str < '0' || '9' < *str)
            return (0);
        str++;
    }
    return (1);
}

int validate_arguments(int argc, char **argv)
{
    int i = 1;
    while (i < argc)
    {
        if (!is_numeric(argv[i]))
            return (1);
        i++;
    }
    return (0);
}

int main(int ac, char **av)
{
    t_table table;

    if (ac != 5 && ac != 6)
    {        
        printf("./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philo_must_eat]\n");
        return(1);
    }
    if (validate_arguments(ac, av) != 0)
    {
        printf("Error: Invalid arguments. All arguments must be positive integers.\n");
        return (1);
    }
     if (init_table(&table, ac, av) != 0)
    {
        printf("Error: Initialization failed.\n");
        return (1);
    }
     if (start_simulation(&table) != 0)
    {
        printf("Error: Failed to start simulation.\n");
        return (1);
    }
    return(0);
}