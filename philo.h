#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
typedef struct s_philo
{
	// int	time_to_die;
	// int	time_to_eat;
	// int	time_to_sleep;
	// int	all_time;
	int	nbr_philo;
	int count;
	pthread_mutex_t	forks;
}		t_philo;

int		ft_atoi(const char *str);
int		handle_error(char *str);

#endif