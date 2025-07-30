/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:15:44 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 05:15:46 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* 標準ライブラリのインクルード */
# include <pthread.h> // pthread関数用
# include <stdio.h>   // printf用
# include <stdlib.h>  // malloc, free用
# include <unistd.h>  // usleep用
# include <sys/time.h> // gettimeofday用

/* --- 課題で規定されたデータ構造 --- */
// ここに t_philosopher と t_table の構造体定義を置く
typedef struct s_table t_table; // 先行宣言

// 各哲学者の情報をまとめる構造体
typedef struct s_philosopher
{
    int             id;                 // 哲学者の番号
    long            last_meal_time;     // 最後に食事した時刻
    int             eat_count;          // 食事した回数
    pthread_t       thread;             // この哲学者を実行するスレッド
    struct s_table  *table;             // 全体情報をまとめた構造体へのポインタ
}   t_philosopher;

// シミュレーション全体の情報をまとめる構造体
typedef struct s_table
{
    int             num_philosophers;   // 哲学者の数
    long            time_to_die;        // 死亡までの時間
    long            time_to_eat;        // 食事時間
    long            time_to_sleep;      // 睡眠時間
    int             must_eat_count;     // 規定食事回数
    long            start_time;         // シミュレーション開始時刻
    t_philosopher   *philosophers;      // 哲学者たちの配列
    pthread_mutex_t *forks;             // フォーク（ミューテックス）の配列
    // ... その他、状態管理用の変数 (例: 死亡フラグなど)
    int             simulation_should_end; // シミュレーション終了フラグ
    pthread_mutex_t print_lock;          // printfを保護するミューテックス
    pthread_mutex_t meal_lock;           // 食事関連の変数を保護するミューテックス
}   t_table;

// utils.c
long    ft_atoi(const char *str);
long    get_current_time(void);
void eat_action(t_philosopher *philo);
void print_status(t_philosopher *philo, const char *status);

// init.c
int     init_table(t_table *table, int ac, char **av);

// simulation.c
int     start_simulation(t_table *table);
void monitor_philosophers(t_table *table);
void *philosopher_routine(void *arg);

#endif