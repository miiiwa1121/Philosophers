/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtsubasa <mtsubasa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 05:16:13 by mtsubasa          #+#    #+#             */
/*   Updated: 2025/07/31 05:24:22 by mtsubasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	ft_atoi(const char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_philosopher *philo, const char *status)
{
	long	current_time;
	int		is_dead_message;

	is_dead_message = (status[0] == 'd');
	pthread_mutex_lock(&philo->table->print_lock);
	if (philo->table->simulation_should_end == 0)
	{
		current_time = get_current_time() - philo->table->start_time;
		printf("%ld %d %s\n", current_time, philo->id, status);
		if (is_dead_message)
			philo->table->simulation_should_end = 1;
	}
	pthread_mutex_unlock(&philo->table->print_lock);
}