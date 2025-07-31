/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:54:41 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 16:10:29 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief  デッドロックを回避しながら、哲学者が左右のフォークを両方取る。
 * @param  philo フォークを取る哲学者のポインタ。
 * @note   哲学者のIDが偶数か奇数かでフォークを取る順番を変えることで、
 * 全員が同時に左(または右)のフォークを取って膠着するのを防ぐ。
 */
static void	take_forks(t_philosopher *philo)
{
	int	left_fork_id;
	int	right_fork_id;

	// 哲学者IDは1から始まるが、配列のインデックスは0からなので調整する
	left_fork_id = philo->id - 1;
	// 最後の哲学者の右隣は最初の哲学者なので、剰余演算で円卓を表現する
	right_fork_id = philo->id % philo->table->num_philosophers;

	// 偶数IDの哲学者は「左→右」の順でフォークを取る
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->table->forks[left_fork_id]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->table->forks[right_fork_id]);
		print_status(philo, "has taken a fork");
	}
	// 奇数IDの哲学者は「右→左」の順でフォークを取る
	else
	{
		pthread_mutex_lock(&philo->table->forks[right_fork_id]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->table->forks[left_fork_id]);
		print_status(philo, "has taken a fork");
	}
}

/**
 * @brief  哲学者が持っている左右のフォークを両方置く。
 * @param  philo フォークを置く哲学者のポインタ。
 */
static void	drop_forks(t_philosopher *philo)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id - 1;
	right_fork_id = philo->id % philo->table->num_philosophers;
	pthread_mutex_unlock(&philo->table->forks[left_fork_id]);
	pthread_mutex_unlock(&philo->table->forks[right_fork_id]);
}

/**
 * @brief 食事の一連の行動（フォークを取る、食べる、フォークを置く）をまとめた関数。
 * @param philo 食事をする哲学者のポインタ。
 */
void	eat_action(t_philosopher *philo)
{
	// 1. 左右のフォークを取る
	take_forks(philo);

	// 2. 食事状態を表示し、食事関連のデータを更新する
	print_status(philo, "is eating");
	// meal_lockで保護し、last_meal_timeとeat_countへの競合を防ぐ
	pthread_mutex_lock(&philo->table->meal_lock);
	philo->last_meal_time = get_current_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->table->meal_lock);

	// 3. 食事時間だけ待機する
	usleep(philo->table->time_to_eat * 1000);

	// 4. 両方のフォークを置く
	drop_forks(philo);
}

/**
 * @brief  各哲学者のスレッドが実行するメインの関数。
 * シミュレーションが終了するまで「食事、睡眠、思考」のサイクルを繰り返す。
 * @param  arg pthread_createから渡される、t_philosopher構造体へのポインタ。
 * @return NULLを返す。
 */
void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_table			*table;

	// void*型で渡された引数を、元のt_philosopher*型にキャストして使う
	philo = (t_philosopher *)arg;
	table = philo->table;

	// 奇数IDの哲学者の開始時間を少し遅らせる。
	// これにより、全スレッドが一斉に同じ動作を始めるのを防ぎ、競合の機会を減らす。
	if (philo->id % 2 != 0)
		usleep(1000);

	// 無限ループ。ループの脱出は内部のbreakで行う。
	while (1)
	{
		// 終了フラグが立っているか、安全にチェックする。
		pthread_mutex_lock(&table->print_lock);
		if (table->simulation_should_end)
		{
			// 終了フラグが立っていたら、ロックを解除してループを抜ける。
			pthread_mutex_unlock(&table->print_lock);
			break ;
		}
		pthread_mutex_unlock(&table->print_lock);

		// 哲学者の行動サイクル
		eat_action(philo);
		print_status(philo, "is sleeping");
		usleep(table->time_to_sleep * 1000);
		print_status(philo, "is thinking");
	}
	return (NULL);
}