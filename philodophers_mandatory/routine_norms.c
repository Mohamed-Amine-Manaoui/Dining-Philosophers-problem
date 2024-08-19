/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_norms.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:16:45 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/14 16:33:04 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_a_fork(t_philo *philo, size_t current_t)
{
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->deads)
	{
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		return ;
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_YELLOW "%zu %d take a fork !!\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
}

void	lock_a_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	take_a_fork(philo, get_current_time());
	pthread_mutex_lock(philo->r_fork);
	take_a_fork(philo, get_current_time());
}

void	death_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->deads)
	{
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		return ;
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
}

int	is_one_philo(t_philo *philo)
{
	if (philo->help->nbr_philo == 1)
	{
		pthread_mutex_lock(&philo->help->mutex_data);
		printf(COLOR_YELLOW "%zu %d take a fork !!\n" COLOR_RESET,
			get_current_time() - philo->help->start, philo->help->philo->id);
		pthread_mutex_unlock(&philo->help->mutex_data);
		ft_msleep(philo->help->time_to_die);
		return (1);
	}
	return (0);
}
