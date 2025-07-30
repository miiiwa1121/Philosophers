/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:16:08 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 05:16:09 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void monitor_philosophers(t_table *table)
{
    int i;
    int all_philos_have_eaten_enough;

    // シミュレーションが終了するまでループを続ける
    while (1)
    {
        i = 0;
        all_philos_have_eaten_enough = 1; // まずは全員が食べ終わったと仮定する

        // --- 全ての哲学者を順にチェックするループ ---
        while (i < table->num_philosophers)
        {
            // meal_lockで保護し、安全に哲学者の食事関連データにアクセスする
            pthread_mutex_lock(&table->meal_lock);

            // 死亡チェック：最後の食事から規定時間が経過したか？
            if (get_current_time() - table->philosophers[i].last_meal_time > table->time_to_die)
            {
                // 死亡していたら、print_statusを呼ぶ。
                // print_status関数内で死亡メッセージの表示と終了フラグの設定が行われる。
                print_status(&table->philosophers[i], "died");
            }

            // 規定食事回数チェック：まだ食事が必要な哲学者がいるか？
            if (table->must_eat_count != -1 && table->philosophers[i].eat_count < table->must_eat_count)
            {
                all_philos_have_eaten_enough = 0; // 食事が必要な人がいた
            }
            
            pthread_mutex_unlock(&table->meal_lock);
            i++;
        }

        // --- シミュレーションの終了条件をチェック ---

        // 規定食事回数が設定されていて、かつ全員が食べ終わった場合
        if (table->must_eat_count != -1 && all_philos_have_eaten_enough)
        {
            // print_lockで保護して、全スレッド共通の終了フラグを立てる
            pthread_mutex_lock(&table->print_lock);
            table->simulation_should_end = 1;
            pthread_mutex_unlock(&table->print_lock);
        }

        // 共有の終了フラグが立っているか確認する
        pthread_mutex_lock(&table->print_lock);
        if (table->simulation_should_end)
        {
            // フラグが立っていたら、監視役のループを終了する
            pthread_mutex_unlock(&table->print_lock);
            break;
        }
        pthread_mutex_unlock(&table->print_lock);

        // CPUを過剰に消費しないよう、短い待機時間を入れる
        usleep(1000);
    }
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_table       *table = philo->table;

// 奇数IDの哲学者は少し待ってから開始し、処理をずらす
    if (philo->id % 2 != 0)
        usleep(1000);

    // 共有の終了フラグが立つまでループを続ける
    while (table->simulation_should_end == 0)
    {
        eat_action(philo);
        
        // 睡眠アクション
        print_status(philo, "is sleeping");
        usleep(table->time_to_sleep * 1000);

        // 思考アクション
        print_status(philo, "is thinking");
    }

    return (NULL);
}

int start_simulation(t_table *table)
{
    int i;
    printf("DEBUG: Starting simulation in ISOLATION MODE.\n");

    i = 0;
    // 全ての哲学者スレッドを作成
    while (i < table->num_philosophers)
    {
        if (pthread_create(&table->philosophers[i].thread, NULL, &philosopher_routine, &table->philosophers[i]) != 0)
            return (1); // エラー
        i++;
    }

    // メインスレッドが監視役になる
    monitor_philosophers(table);

    i = 0;
    // 全てのスレッドが終了するのを待つ
    while (i < table->num_philosophers)
    {
        if (pthread_join(table->philosophers[i].thread, NULL) != 0)
            return (1); // エラー
        i++;
    }
    printf("DEBUG: Isolation mode test finished.\n");
    return (0);
}