/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:45:10 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/05 00:43:42 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	valid_nbr(t_help *help, char **av)
{
	if (help->nbr_philo <= 0 || (av[5] && help->nbr_meals <= 0))
		return (0);
	if (help->time_to_die < 0 || help->time_to_eat < 0
		|| help->time_to_sleep < 0)
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	t_help	*help;

	help = malloc(sizeof(t_help));
	if (ac == 5 || ac == 6)
	{
		if (valid_args(av, help) == 1)
			return (free(help), printf("INVALID ARGUMENT !!!!\n"), 0);
		if (valid_nbr(help, av) == 0)
			return (free(help), 0);
		help->start = get_current_time();
		help->deads = 0;
		help->philo_meals = help->nbr_philo;
		help->flag_meals = 1;
		init_mutex(help);
		init_philo(help);
		__monitor__(help);
		join_philo(help);
		free(help->forks);
		free(help->t1);
		free(help->philo);
	}
	else
		printf("number for argument invalid !!!\n");
	free(help);
	return (0);
}

// int	main(void)
// {
// 	t_help	*help;

// 	help = malloc(sizeof(t_help));
// 	help->start = get_current_time();
// 	help->deads = 0;
// 	help->philo_meals = nbr_philo;
// 	// for now before parsing
// 	help->flag_meals = 1;
// 	init_mutex(help);
// 	init_philo(help);
// 	__monitor__(help);
// 	join_philo(help);
// 	return (0);
// }

// philo ==> thread
// forks ==> mutex