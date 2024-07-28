/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:17:23 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/07/28 22:18:21 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eating(t_philo *philo, size_t current_t)
{
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->deads)
	{
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		return (1);
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->flag_meals == 1)
	{
		philo->meals--;
		if (philo->meals == 0)
			philo->help->philo_meals--;
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_GREEN "%zu %d EATING\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
	pthread_mutex_lock(&philo->help->mutex_monitor);
	philo->last_time_eat = get_current_time();
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	ft_msleep(philo->help->time_to_eat);
	return (0);
}

int	sleeping(t_philo *philo, size_t current_t)
{
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->deads)
	{
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		return (1);
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_GREEN "%zu %d SLEEPING\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
	ft_msleep(philo->help->time_to_sleep);
	return (0);
}

int	thinking(t_philo *philo, size_t current_t)
{
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->deads)
	{
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		return (1);
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_GREEN "%zu %d THINKING\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
	return (0);
}

void	died(t_philo *philo, size_t current_t)
{
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_RED "%zu %d died\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
}

void	*ss(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(60);
	while (1)
	{
		ee(philo);
		dd(philo);
		if (eating(philo, get_current_time()) == 1)
		{
			pthread_mutex_unlock(philo->r_fork);
			pthread_mutex_unlock(philo->l_fork);
			return (NULL);
		}
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		if (sleeping(philo, get_current_time()) == 1)
			return (NULL);
		if (thinking(philo, get_current_time()) == 1)
			return (NULL);
		ft_msleep((philo->help->time_to_die - (get_current_time()
					- philo->last_time_eat)) / 2);
	}
	return (NULL);
}
