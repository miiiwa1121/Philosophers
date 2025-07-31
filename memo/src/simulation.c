/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:16:08 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 16:14:09 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief  哲学者ごとにスレッドを作成し、それらが終了するのを待つ。
 * @param  table テーブル構造体へのポインタ。
 * @return 成功した場合は0、失敗した場合は1を返す。
 * @note   この関数はシミュレーションの核となるループを持つ。
 * メインスレッドは、全哲学者のスレッドを作成した後、監視役の役割を担う。
 */
static int	create_and_join_threads(t_table *table)
{
	int	i;

	i = 0;
	// 哲学者一人ひとりに対してスレッドを作成するループ
	while (i < table->num_philosophers)
	{
		// pthread_createで新しいスレッドを開始する。
		// philosopher_routine関数を実行し、引数としてその哲学者のデータへのポインタを渡す。
		if (pthread_create(&table->philosophers[i].thread, NULL,
				&philosopher_routine, &table->philosophers[i]) != 0)
			return (1); // スレッド作成に失敗したら1を返す
		i++;
	}
	// 全てのスレッドを作成した後、メインスレッドは監視役になる。
	// この関数は、シミュレーションが終了するまで処理をブロックする。
	monitor_philosophers(table);
	i = 0;
	// シミュレーション終了後、全ての哲学者のスレッドが完了するのを待つ。
	// これはクリーンなシャットダウンのために重要。
	while (i < table->num_philosophers)
	{
		if (pthread_join(table->philosophers[i].thread, NULL) != 0)
			return (1); // スレッドの合流に失敗したら1を返す
		i++;
	}
	return (0);
}

/**
 * @brief  シミュレーションを開始するためのメインのエントリーポイント。
 * @param  table 初期化済みのテーブル構造体へのポインタ。
 * @return 成功した場合は0、失敗した場合は1を返す。
 * @note   哲学者が一人の場合という特殊なエッジケースを扱う。
 */
int	start_simulation(t_table *table)
{
	// エッジケース：哲学者が一人の場合、フォークは一本しかない。
	// その哲学者は二本目のフォークを取ることができず、必ず餓死する。
	if (table->num_philosophers == 1)
	{
		// この単純なシナリオを手動で処理する。
		table->start_time = get_current_time();
		print_status(&table->philosophers[0], "has taken a fork");
		usleep(table->time_to_die * 1000); // 餓死するまで待機
		print_status(&table->philosophers[0], "died");
		return (0);
	}
	// それ以外の全ての場合、スレッドを作成してシミュレーションを進める。
	return (create_and_join_threads(table));
}
