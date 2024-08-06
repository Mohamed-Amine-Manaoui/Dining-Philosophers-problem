/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 21:25:33 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/06 21:25:58 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

// #define NBR_PHILO 5
// #define TIME_TO_DIE 180
// #define TIME_TO_EAT 60
// #define TIME_TO_SLEEP 60

// #define NBR_PHILO 4
// #define TIME_TO_DIE 310
// #define TIME_TO_EAT 200
// #define TIME_TO_SLEEP 100

# define NBR_PHILO 3
# define TIME_TO_DIE 180
# define TIME_TO_EAT 60
# define TIME_TO_SLEEP 60
# define NBR_MEALS 1

struct	s_help;
typedef struct s_philo
{
	int				id;
	size_t			last_time_eat;
	size_t			meals;
	pthread_t		t1;
	struct s_help	*help;
}					t_philo;

typedef struct s_help
{
	sem_t			*sem_forks;
	sem_t			*sem_write;
	sem_t			*sem_monitor;
	sem_t			*sem_eat;
	size_t			start;
	size_t			flag_meals;
	int				philo_meals;
	pid_t			*pids;
	struct s_philo	*philo;
}					t_help;

# define COLOR_RESET "\033[0m"
# define COLOR_RED "\033[31m"
# define COLOR_GREEN "\033[32m"
# define COLOR_YELLOW "\033[33m"
# define COLOR_BLUE "\033[34m"

// inisialization
int					init_semaphore(t_help *help);
void				init_philo(t_help *help);
// monitor
void				*monitor_die(void *arg);
size_t				get_current_time(void);
int					ft_msleep(size_t milliseconds);
// routine
void				routine(t_philo *philo);

#endif