/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:45:10 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/08 23:39:45 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	valid_nbr(t_help *help, char **av)
{
	if (help->nbr_philo <= 0 || (av[5] && help->nbr_meals <= 0))
		return (1);
	if (help->time_to_die < 0 || help->time_to_eat < 0
		|| help->time_to_sleep < 0)
		return (1);
	return (0);
}

int	valid_data(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (av[i][0] == '\0' || count_spaces(av[i], 32) == ft_strlen(av[i])
			|| (av[i][0] == '-' && !ft_isdigit(av[i][1])))
		{
			write(2, "Error\n", 6);
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_help	*help;

	help = malloc(sizeof(t_help));
	if (ac == 5 || ac == 6)
	{
		if (valid_data(ac, av) == 1)
			return (free(help), printf("INVALID DATA !!!!\n"), 0);
		if (valid_args(av, ac, help) == 1)
			return (free(help), printf("INVALID ARGUMENT !!!!\n"), 0);
		if (valid_nbr(help, av) == 1)
			return (free(help), printf("Error, Negative Number\n"), 0);
		help->start = get_current_time();
		help->philo_meals = help->nbr_philo;
		help->deads = 0;
		help->flag_meals = 1;
		init_mutex(help);
		init_philo(help);
		__monitor__(help);
		join_philo(help);
		free(help->forks);
		(free(help->t1), free(help->philo));
	}
	else
		printf("number for argument invalid !!!\n");
	return (free(help), 0);
}
