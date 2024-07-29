/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:04:32 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/07/29 06:29:05 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_mutex(t_help *help)
{
	int	i;

	i = 0;
	help->forks = malloc(help->nbr_philo * sizeof(pthread_mutex_t));
	while (i < help->nbr_philo)
	{
		pthread_mutex_init(&help->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&help->mutex_data, NULL);
	pthread_mutex_init(&help->mutex_monitor, NULL);
	pthread_mutex_init(&help->mutex_dead, NULL);
}

void	init_philo(t_help *help)
{
	int	i;

	help->t1 = malloc(help->nbr_philo * sizeof(pthread_t));
	help->philo = malloc(help->nbr_philo * sizeof(t_philo));
	i = 0;
	help->philo_meals = help->nbr_philo;
	while (i < help->nbr_philo)
	{
		if (help->flag_meals)
			help->philo[i].meals = help->nbr_meals;
		help->philo[i].id = i + 1;
		help->philo[i].help = help;
		help->philo[i].last_time_eat = get_current_time();
		help->philo[i].l_fork = &help->forks[i];
		help->philo[i].r_fork = &help->forks[(i + 1) % help->nbr_philo];
		if (i == help->nbr_philo - 1)
		{
			help->philo[i].r_fork = &help->forks[i];
			help->philo[i].l_fork = &help->forks[(i + 1) % help->nbr_philo];
		}
		i++;
	}
	i = -1;
	while (++i < help->nbr_philo)
		pthread_create(&help->t1[i], NULL, routine, &help->philo[i]);
}

void	join_philo(t_help *help)
{
	int	i;

	i = 0;
	while (i < help->nbr_philo)
	{
		pthread_join(help->t1[i], NULL);
		i++;
	}
}
