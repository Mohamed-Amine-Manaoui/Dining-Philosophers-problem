/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inisialization_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 20:37:03 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/18 12:15:17 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_semaphore(t_help *help)
{
	help->philo = malloc(help->nbr_philo * sizeof(t_philo));
	if (!help->philo)
		return (1);
	help->pids = malloc(help->nbr_philo * sizeof(pid_t));
	if (!help->pids)
		return (1);
	help->start = get_current_time();
	sem_unlink("forks");
	help->sem_forks = sem_open("forks", O_CREAT | O_EXCL, 0644,
			help->nbr_philo);
	sem_unlink("write");
	help->sem_write = sem_open("write", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("dead");
	help->sem_dead = sem_open("dead", O_CREAT | O_EXCL, 0644, 0);
	sem_unlink("eat");
	help->sem_eat = sem_open("eat", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("monitor");
	help->sem_monitor = sem_open("monitor", O_CREAT | O_EXCL, 0644, 1);
	if (help->sem_forks == SEM_FAILED || help->sem_write == SEM_FAILED
		|| help->sem_eat == SEM_FAILED || help->sem_monitor == SEM_FAILED
		|| help->sem_dead == SEM_FAILED)
		return (1);
	return (0);
}

void	init_philo(t_help *help)
{
	int	i;

	i = 0;
	while (i < help->nbr_philo)
	{
		if (help->flag_meals)
			help->philo[i].meals = help->nbr_meals;
		help->philo[i].id = i + 1;
		help->philo[i].help = help;
		i++;
	}
}

int	is_one_philo(t_philo *philo)
{
	if (philo->help->nbr_philo == 1)
	{
		sem_wait(philo->help->sem_write);
		printf(COLOR_YELLOW "%zu %d take a fork !!\n" COLOR_RESET,
			get_current_time() - philo->help->start, philo->help->philo->id);
		sem_post(philo->help->sem_write);
		ft_msleep(philo->help->time_to_die);
		sem_wait(philo->help->sem_write);
		printf(COLOR_RED "%zu %d died\n" COLOR_RESET, get_current_time()
			- philo->help->start, philo->id);
		sem_post(philo->help->sem_write);
		return (1);
	}
	return (0);
}
