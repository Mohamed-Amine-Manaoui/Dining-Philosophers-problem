/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 20:33:47 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/06 21:24:55 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	go_philo(t_help *help)
{
	int	i;

	i = 0;
	while (i < NBR_PHILO)
	{
		help->philo[i].last_time_eat = get_current_time();
		help->pids[i] = fork();
		if (help->pids[i] == 0)
		{
			pthread_create(&help->philo[i].t1, NULL, monitor_die,
				&help->philo[i]);
			routine(&help->philo[i]);
		}
		i++;
	}
}

void	kill_pids(t_help *help)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < NBR_PHILO)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			i = 0;
			while (i < NBR_PHILO)
			{
				kill(help->pids[i], 15);
				i++;
			}
		}
		i++;
	}
}

int	main(void)
{
	t_help	*help;

	help = malloc(sizeof(t_help));
	if (init_semaphore(help) == 1)
		return (free(help), 0);
	help->philo_meals = NBR_PHILO;
	help->flag_meals = 0;
	init_philo(help);
	go_philo(help);
	kill_pids(help);
	sem_post(help->sem_write);
}
