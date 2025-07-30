/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:15:51 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 05:15:53 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// 自作の ft_atoi や get_current_time が必要
int init_table(t_table *table, int ac, char **av)
{
    int i;

    // 1. 引数を構造体に格納
    table->num_philosophers = ft_atoi(av[1]);
    table->time_to_die = ft_atoi(av[2]);
    table->time_to_eat = ft_atoi(av[3]);
    table->time_to_sleep = ft_atoi(av[4]);
    if (ac == 6)
        table->must_eat_count = ft_atoi(av[5]);
    else
        table->must_eat_count = -1; // 任意引数がない場合の目印

    // 2. メモリ確保
    table->philosophers = malloc(sizeof(t_philosopher) * table->num_philosophers);
    table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philosophers);
    if (!table->philosophers || !table->forks)
        return (1); // Malloc error

    // 3. ミューテックスを初期化
    i = 0;
    while (i < table->num_philosophers)
    {
        if (pthread_mutex_init(&table->forks[i], NULL) != 0)
            return (1); // Mutex init error
        i++;
    }

    // 4. 哲学者を初期化 & 5. 開始時刻
    table->start_time = get_current_time(); // gettimeofdayのラッパー関数
    table->simulation_should_end = 0;
    i = 0;
    while (i < table->num_philosophers)
    {
        table->philosophers[i].id = i + 1;
        table->philosophers[i].eat_count = 0;
        table->philosophers[i].last_meal_time = table->start_time;
        table->philosophers[i].table = table;
        i++;
    }
    if (pthread_mutex_init(&table->print_lock, NULL) != 0)
        return (1);
    if (pthread_mutex_init(&table->meal_lock, NULL) != 0)
        return (1);
    return (0); // 成功
}