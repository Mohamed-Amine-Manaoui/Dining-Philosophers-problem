/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:23:18 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/07/28 22:36:59 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_msleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

int	__if_max_meals__(t_help *help)
{
	pthread_mutex_lock(&help->mutex_monitor);
	if (help->philo_meals == 0)
	{
		pthread_mutex_lock(&help->mutex_dead);
		help->deads = 1;
		pthread_mutex_unlock(&help->mutex_dead);
		pthread_mutex_unlock(&help->mutex_monitor);
		return (0);
	}
	pthread_mutex_unlock(&help->mutex_monitor);
	return (1);
}

void	__monitor__(t_help *help)
{
	int			i;
	size_t			current_time;

	while (1)
	{
		if (help->flag_meals == 1 && __if_max_meals__(help) == 0)
			return ;
		i = 0;
		while (i < help->nbr_philo)
		{
			pthread_mutex_lock(&help->mutex_monitor);
			current_time = get_current_time();
			if (current_time - help->philo[i].last_time_eat > help->time_to_die)
			{
				died(&help->philo[i], get_current_time());
				pthread_mutex_lock(&help->mutex_dead);
				help->deads = 1;
				pthread_mutex_unlock(&help->mutex_dead);
				pthread_mutex_unlock(&help->mutex_monitor);
				return ;
			}
			pthread_mutex_unlock(&help->mutex_monitor);
			i++;
		}
	}
}
