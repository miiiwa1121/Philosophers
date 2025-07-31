/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:16:13 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 16:15:22 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief  数字で構成された文字列をlong型の数値に変換する。
 * @param  str 変換対象の文字列。
 * @return 変換後のlong型の数値。
 * @note   この実装は、入力が正の整数であることを前提としている。
 */
long	ft_atoi(const char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	// 文字列の先頭から、文字が'0'から'9'の間である限りループ
	while (str[i] >= '0' && str[i] <= '9')
	{
		// それまでの結果を10倍して桁を上げ、新しい数値を加算する
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

/**
 * @brief  現在の時刻をミリ秒単位で取得する。
 * @return ミリ秒単位の現在時刻。
 */
long	get_current_time(void)
{
	// `timeval`構造体は、秒(tv_sec)とマイクロ秒(tv_usec)を保持する
	struct timeval	tv;

	// gettimeofdayで現在の時刻を`tv`に格納する
	gettimeofday(&tv, NULL);
	// 秒を1000倍してミリ秒に変換し、マイクロ秒を1000で割ってミリ秒に変換したものを足す
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief  哲学者の状態変化を、競合を避けながら安全に表示する。
 * @param  philo 状態を報告する哲学者。
 * @param  status 表示する状態の文字列 ("is eating", "died"など)。
 * @note   この関数は、シミュレーション終了後の不要なメッセージを抑制する「門番」の役割も担う。
 */
void	print_status(t_philosopher *philo, const char *status)
{
	long	current_time;
	int		is_dead_message;

	// 表示するステータスが"died"かどうかを判定する
	// 'd'で始まるのは"died"だけなので、最初の文字で判定できる
	is_dead_message = (status[0] == 'd');
	// printfが複数のスレッドから同時に呼ばれるのを防ぐため、出力をロックする
	pthread_mutex_lock(&philo->table->print_lock);
	// ★★★ このif文が競合状態を防ぐ鍵 ★★★
	// シミュレーションがまだ続いている場合にのみ、メッセージを表示する
	if (philo->table->simulation_should_end == 0)
	{
		current_time = get_current_time() - philo->table->start_time;
		printf("%ld %d %s\n", current_time, philo->id, status);
		// もしこのメッセージが死亡通知であれば、ここで終了フラグを立てる
		// このロック内でフラグを立てることで、他のスレッドが同時に死亡メッセージを出力するのを防ぐ
		if (is_dead_message)
			philo->table->simulation_should_end = 1;
	}
	// 必ずロックを解放する
	pthread_mutex_unlock(&philo->table->print_lock);
}
