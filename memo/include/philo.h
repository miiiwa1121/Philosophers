/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:15:44 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 16:09:37 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* --- 必要な標準ライブラリのインクルード --- */
// スレッド操作（作成、合流など）のための関数群
# include <pthread.h>
// 標準入出力（printfなど）のための関数群
# include <stdio.h>
// メモリの動的確保（malloc, free）や文字列変換（atoi）のための関数群
# include <stdlib.h>
// usleep（マイクロ秒単位での待機）やwriteのための関数群
# include <unistd.h>
// gettimeofday（高精度な現在時刻取得）のための関数群
# include <sys/time.h>

// 構造体 s_table の存在をコンパイラに先に知らせる（前方宣言）。
// これにより、t_philosopher 内で t_table* を問題なく使えるようになる。
typedef struct s_table	t_table;

/**
 * @struct s_philosopher
 * @brief  一人の哲学者の状態を管理するための構造体
 */
typedef struct s_philosopher
{
	int			id;                 // 哲学者の識別番号 (1から始まる)
	long		last_meal_time;     // 最後に食事を開始した時刻
	int			eat_count;          // この哲学者が食事をした回数
	pthread_t	thread;             // この哲学者に対応するスレッドのID
	t_table		*table;             // 全体の設定を共有するためのテーブルへのポインタ
}	t_philosopher;

/**
 * @struct s_table
 * @brief  シミュレーション全体のルールと状態を管理するための構造体
 */
typedef struct s_table
{
	int					num_philosophers;   // 哲学者の総数
	long				time_to_die;        // 死亡までの時間 (ミリ秒)
	long				time_to_eat;        // 食事にかかる時間 (ミリ秒)
	long				time_to_sleep;      // 睡眠にかかる時間 (ミリ秒)
	int					must_eat_count;     // (任意引数) 各哲学者が食事すべき回数
	long				start_time;         // シミュレーション開始時刻
	t_philosopher		*philosophers;      // 全哲学者のデータ配列へのポインタ
	pthread_mutex_t		*forks;             // 全フォーク（ミューテックス）の配列へのポインタ
	int					simulation_should_end; // シミュレーション終了フラグ (0:継続, 1:終了)
	pthread_mutex_t		print_lock;         // printfが同時に呼ばれるのを防ぐためのミューテックス
	pthread_mutex_t		meal_lock;          // last_meal_timeやeat_countへの同時アクセスを防ぐミューテックス
}	t_table;

/* --- 関数プロトタイプ宣言 --- */
// これにより、どの.cファイルからでも他の.cファイルの関数を呼び出せるようになる

/* init.c */
// シミュレーション全体のテーブルを初期化する
int		init_table(t_table *table, int ac, char **av);

/* main.c */
//確保したメモリやミューテックスを全て解放する
void	cleanup(t_table *table);

/* simulation.c */
// シミュレーションを開始し、スレッドの生成と監視役の起動を行う
int		start_simulation(t_table *table);

/* actions.c */
// 各哲学者が実行するメインの行動ルーチン
void	*philosopher_routine(void *arg);
// 食事の一連の動作（フォーク確保、食事、フォーク解放）
void	eat_action(t_philosopher *philo);

/* monitor.c */
// 全ての哲学者の死亡や食事回数を監視する
void	monitor_philosophers(t_table *table);

/* utils.c */
// 文字列をlong型に変換する
long	ft_atoi(const char *str);
// 現在時刻をミリ秒単位で取得する
long	get_current_time(void);
// 哲学者の状態変化を指定されたフォーマットで表示する
void	print_status(t_philosopher *philo, const char *status);

#endif