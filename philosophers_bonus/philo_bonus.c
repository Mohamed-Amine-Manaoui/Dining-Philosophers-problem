/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 20:33:47 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/11 07:11:37 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	valid_nbr(t_help *help, char **av)
{
	if (help->nbr_philo <= 0 || (av[5] && help->nbr_meals <= 0))
		return (0);
	if (help->time_to_die < 0 || help->time_to_eat < 0
		|| help->time_to_sleep < 0)
		return (0);
	return (1);
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

int	main(int ac, char **av)
{
	t_help	*help;

	help = malloc(sizeof(t_help));
	if (ac == 5 || ac == 6)
	{
		if (valid_data(ac, av) == 1)
			return (free(help), printf("INVALID DATA !!!!\n"), 0);
		if (valid_args(av, help) == 1)
			return (free(help), printf("INVALID ARGUMENT !!!!\n"), 0);
		if (valid_nbr(help, av) == 0)
			return (free(help), 0);
		if (init_semaphore(help) == 1)
			return (free(help), 0);
		help->philo_meals = help->nbr_meals;
		help->flag_meals = 0;
		init_philo(help);
		go_philo(help);
		kill_pids(help);
		sem_post(help->sem_write);
	}
	else
		printf("number for argument invalid !!!\n");
	return (free(help), 0);
}
