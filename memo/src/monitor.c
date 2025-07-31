/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:54:55 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 16:12:32 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief  指定された哲学者が死亡したかチェックする。
 * @param  philo チェック対象の哲学者。
 * @return 死亡している場合は1、生存している場合は0を返す。
 * @note   この関数は、哲学者の食事関連データにアクセスするため、meal_lockで保護する。
 */
static int	check_death(t_philosopher *philo)
{
	// meal_lockで保護し、last_meal_timeへの安全なアクセスを保証する。
	pthread_mutex_lock(&philo->table->meal_lock);

	// 現在時刻と最後の食事時刻の差が、指定された死亡時間を超えたか判定。
	if (get_current_time() - philo->last_meal_time > philo->table->time_to_die)
	{
		// 死亡していたら、死亡ステータスを表示する。
		// この中で、競合を避けるために終了フラグが立てられる。
		print_status(philo, "died");
		// 判定が終わったら、必ずロックを解放する。
		pthread_mutex_unlock(&philo->table->meal_lock);
		return (1);
	}
	// 生存している場合も、ロックを解放してから関数を抜ける。
	pthread_mutex_unlock(&philo->table->meal_lock);
	return (0);
}

/**
 * @brief  (任意引数が指定された場合) 全ての哲学者が規定回数食事を終えたかチェックする。
 * @param  table テーブル構造体へのポインタ。
 * @return 全員が食べ終わっていたら1、そうでなければ0を返す。
 */
static int	check_all_eaten(t_table *table)
{
	int	i;
	int	all_eaten;

	// 任意引数が指定されていない場合は、このチェックは不要。
	if (table->must_eat_count == -1)
		return (0);

	// まずは全員が食べ終わったと仮定する。
	all_eaten = 1;
	i = 0;
	while (i < table->num_philosophers)
	{
		// meal_lockで保護し、各哲学者のeat_countに安全にアクセスする。
		pthread_mutex_lock(&table->meal_lock);
		// 一人でも規定回数に達していない哲学者がいれば、フラグを0にする。
		if (table->philosophers[i].eat_count < table->must_eat_count)
			all_eaten = 0;
		pthread_mutex_unlock(&table->meal_lock);
		i++;
	}
	// ループ後、フラグが1のままなら、全員が食べ終わったということ。
	if (all_eaten)
	{
		// print_lockで保護し、シミュレーション終了フラグを立てる。
		pthread_mutex_lock(&table->print_lock);
		table->simulation_should_end = 1;
		pthread_mutex_unlock(&table->print_lock);
		return (1);
	}
	return (0);
}

/**
 * @brief  メインスレッドが実行する監視役のメインループ。
 * @param  table テーブル構造体へのポインタ。
 * @note   この関数は、誰かが死ぬか、全員が食べ終わるまでループし続ける。
 */
void	monitor_philosophers(t_table *table)
{
	int	i;

	while (1)
	{
		i = 0;
		// 全ての哲学者を順番にチェックする
		while (i < table->num_philosophers)
		{
			// i番目の哲学者が死んでいないかチェックする
			if (check_death(&table->philosophers[i]))
				return ; // 誰かが死んだら、監視役の役目は終わりなので即座に終了
			i++;
		}
		// 全員が規定回数食べ終わっていないかチェックする
		if (check_all_eaten(table))
			return ; // 全員食べ終わったら、同様に終了

		// CPUを過剰に消費しないよう、短いスリープを挟む
		usleep(1000);
	}
}
