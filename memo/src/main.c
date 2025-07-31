/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:15:56 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 16:11:54 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief  与えられた文字列が数字のみで構成されているかチェックする。
 * @param  str チェックする文字列。
 * @return 数字のみの場合は1、それ以外の文字が含まれていれば0を返す。
 */
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

/**
 * @brief  プログラムの引数が正しいか検証する。
 * @param  argc 引数の数。
 * @param  argv 引数の配列。
 * @return 引数が有効な場合は0、無効な場合は1を返す。
 */
static int	validate_arguments(int argc, char **argv)
{
	int	i;

	// 引数の数が5個または6個でない場合はエラー
	if (argc != 5 && argc != 6)
	{
		// 正しい使用方法をエラーメッセージとして表示
		printf("Usage: ./philo <num_of_philos> <time_to_die> <time_to_eat> "
			"<time_to_sleep> [num_of_times_each_philo_must_eat]\n");
		return (1);
	}
	i = 1; // プログラム名(av[0])はスキップ
	while (i < argc)
	{
		// 各引数が数字のみで構成されているかチェック
		if (!is_numeric(argv[i]))
		{
			printf("Error: All arguments must be positive integers.\n");
			return (1);
		}
		i++;
	}
	// 全ての検証をパスしたら成功を返す
	return (0);
}

/**
 * @brief  シミュレーション終了時に、確保した全リソースを解放する。
 * @param  table 解放するリソースを保持しているテーブル構造体へのポインタ。
 * @note   メモリリークやミューテックスの未破棄を防ぐための重要な後片付け処理。
 */
void	cleanup(t_table *table)
{
	int	i;

	i = 0;
	// 各フォークのミューテックスを破棄
	while (i < table->num_philosophers)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	// その他の共有ミューテックスを破棄
	pthread_mutex_destroy(&table->print_lock);
	pthread_mutex_destroy(&table->meal_lock);
	// mallocで確保したメモリを解放
	free(table->forks);
	free(table->philosophers);
}

/**
 * @brief  プログラムのエントリーポイント。
 * @param  ac コマンドライン引数の数。
 * @param  av コマンドライン引数の文字列配列。
 * @return プログラムが正常に終了した場合は0、エラーがあった場合は1を返す。
 */
int	main(int ac, char **av)
{
	t_table	table;

	// 1. 引数を検証する
	if (validate_arguments(ac, av) != 0)
		return (1);

	// 2. テーブル構造体を初期化する
	if (init_table(&table, ac, av) != 0)
	{
		printf("Error: Initialization failed.\n");
		return (1);
	}

	// 3. シミュレーションを開始する
	if (start_simulation(&table) != 0)
	{
		// シミュレーション開始に失敗した場合も、確保したリソースは解放する
		cleanup(&table);
		printf("Error: Failed to start simulation.\n");
		return (1);
	}

	// 4. シミュレーションが正常に終了したら、リソースを解放する
	cleanup(&table);
	return (0);
}