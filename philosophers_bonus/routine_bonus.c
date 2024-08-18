/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 20:50:35 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/18 15:02:01 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	take_a_fork(t_philo *philo)
{
	sem_wait(philo->help->sem_write);
	printf(COLOR_YELLOW "%zu %d has taken a fork\n" COLOR_RESET,
		get_current_time() - philo->help->start, philo->id);
	sem_post(philo->help->sem_write);
	sem_wait(philo->help->sem_forks);
	sem_wait(philo->help->sem_write);
	printf(COLOR_YELLOW "%zu %d has taken a fork\n" COLOR_RESET,
		get_current_time() - philo->help->start, philo->id);
	sem_post(philo->help->sem_write);
}

void	eating(t_philo *philo)
{
	sem_wait(philo->help->sem_write);
	printf(COLOR_GREEN "%zu %d is eating\n" COLOR_RESET, get_current_time()
		- philo->help->start, philo->id);
	sem_post(philo->help->sem_write);
	sem_wait(philo->help->sem_write);
	philo->last_time_eat = get_current_time();
	sem_post(philo->help->sem_write);
	ft_msleep(philo->help->time_to_eat);
}

void	sleeping(t_philo *philo)
{
	sem_wait(philo->help->sem_write);
	printf(COLOR_BLUE "%zu %d is sleeping\n" COLOR_RESET, get_current_time()
		- philo->help->start, philo->id);
	sem_post(philo->help->sem_write);
	ft_msleep(philo->help->time_to_sleep);
}

void	thinking(t_philo *philo)
{
	sem_wait(philo->help->sem_write);
	printf(COLOR_YELLOW "%zu %d is thinking\n" COLOR_RESET, get_current_time()
		- philo->help->start, philo->id);
	sem_post(philo->help->sem_write);
	ft_msleep(1);
}

void	routine(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(50);
	while (1)
	{
		if (is_one_philo(philo) == 1)
			exit(0);
		sem_wait(philo->help->sem_forks);
		take_a_fork(philo);
		eating(philo);
		sem_post(philo->help->sem_forks);
		sem_post(philo->help->sem_forks);
		if (philo->help->flag_meals == 1)
		{
			philo->meals--;
			if (philo->meals == 0)
			{
				exit(0);
			}
		}
		sleeping(philo);
		thinking(philo);
	}
	printf("exit routine\n");
}
