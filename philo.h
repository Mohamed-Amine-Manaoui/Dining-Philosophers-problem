/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:00:10 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/07/28 22:35:45 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

struct	s_help;
typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	size_t			last_time_eat;
	size_t			meals;
	struct s_help	*help;
}					t_philo;

typedef struct s_help
{
	int				nbr_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_meals;
	pthread_t		*t1;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex_data;
	pthread_mutex_t	mutex_monitor;
	pthread_mutex_t	mutex_dead;
	struct s_philo	*philo;
	int				deads;
	size_t			start;
	size_t			philo_meals;
	size_t			flag_meals;
}					t_help;

# define COLOR_RESET "\033[0m"
# define COLOR_RED "\033[31m"
# define COLOR_GREEN "\033[32m"
# define COLOR_YELLOW "\033[33m"
# define COLOR_BLUE "\033[34m"

int					valid_args(char **av, t_help *help);

// initializasion
void				init_mutex(t_help *help);
void				init_philo(t_help *help);
void				join_philo(t_help *help);
// routine
void				*ss(void *arg);
int					eating(t_philo *philo, size_t current_t);
int					sleeping(t_philo *philo, size_t current_t);
int					thinking(t_philo *philo, size_t current_t);
void				died(t_philo *philo, size_t current_t);
// routine_norms
void				take_a_fork(t_philo *philo, size_t current_t);
void				dd(t_philo *philo);
void				ee(t_philo *philo);
// monitor
size_t				get_current_time(void);
int					ft_msleep(size_t milliseconds);
int					__if_max_meals__(t_help *help);
void				__monitor__(t_help *help);
#endif