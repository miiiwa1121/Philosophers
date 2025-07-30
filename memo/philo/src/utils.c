/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:16:13 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 05:16:14 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long ft_atoi(const char *str)
{
    long result;
    int i;

    result = 0;
    i = 0;
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return (result);
}

long get_current_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void print_status(t_philosopher *philo, const char *status)
{
    long current_time;
    int is_dead_message;

    // "died"という文字列が渡されたかを判定
    is_dead_message = (status[0] == 'd');

    // 出力全体をロックで保護する
    pthread_mutex_lock(&philo->table->print_lock);

    // ★★★ここが最も重要★★★
    // シミュレーションがまだ続いている場合にのみ、メッセージを表示する
    if (philo->table->simulation_should_end == 0)
    {
        current_time = get_current_time() - philo->table->start_time;
        printf("%ld %d %s\n", current_time, philo->id, status);
        
        // もしこのメッセージが死亡通知であれば、ここで終了フラグを立てる
        if (is_dead_message)
        {
            philo->table->simulation_should_end = 1;
        }
    }
    
    // ロックを解除
    pthread_mutex_unlock(&philo->table->print_lock);
}
void eat_action(t_philosopher *philo)
{
    t_table *table = philo->table;
    int left_fork_id;
    int right_fork_id;

    // フォークのIDを決定 (哲学者は1-indexed, 配列は0-indexed)
    left_fork_id = philo->id - 1;
    right_fork_id = philo->id % table->num_philosophers;

    // IDが偶数か奇数かでフォークを取る順番を変える
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
    
    // 食事開始
    print_status(philo, "is eating");
    
    // meal_lockで保護しながら、最後の食事時刻と食事回数を更新
    printf("%ld %d is TRYING to lock meal_lock\n", get_current_time() - philo->table->start_time, philo->id);
    pthread_mutex_lock(&philo->table->meal_lock);
    printf("%ld %d ACQUIRED meal_lock\n", get_current_time() - philo->table->start_time, philo->id);

    philo->last_meal_time = get_current_time();
    philo->eat_count++;

    pthread_mutex_unlock(&philo->table->meal_lock);
    printf("%ld %d RELEASED meal_lock\n", get_current_time() - philo->table->start_time, philo->id);
        
    usleep(philo->table->time_to_eat * 1000); // 食事時間

    // フォークを解放
    pthread_mutex_unlock(&table->forks[left_fork_id]);
    pthread_mutex_unlock(&table->forks[right_fork_id]);
}