/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 21:25:33 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/11 09:35:08 by mmanaoui         ###   ########.fr       */
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
	int				nbr_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_meals;
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

// external function
int					ft_strlen(char *str);
int					ft_isdigit(int c);
int					ft_atoi(const char *str);
int					count_spaces(char *str, char c);
int					valid_data(int ac, char **av);
// inisialization bonus
int					init_semaphore(t_help *help);
void				init_philo(t_help *help);
// monitor bonus
size_t				get_current_time(void);
int					ft_msleep(size_t milliseconds);
void				*monitor_die(void *arg);
// parsing bonus
int					check_alpha(char *av);
int					handle_error(char *str);
void				handle_whitespaces(char **av);
int					valid_args(char **av, t_help *help);
// main project bonus
int					valid_nbr(t_help *help, char **av);
void				kill_pids(t_help *help);
void				go_philo(t_help *help);
// routine bonus
void				take_a_fork(t_philo *philo);
void				eating(t_philo *philo);
void				sleeping(t_philo *philo);
void				thinking(t_philo *philo);
void				routine(t_philo *philo);

#endif