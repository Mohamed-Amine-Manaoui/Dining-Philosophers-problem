#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NBR_PHILO 5

typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}					t_philo;

typedef struct s_help
{
	pthread_t		*t1;
	pthread_mutex_t *forks;
	struct s_philo	*philo;
}					t_help;

void	*ss(void *arg)
{
	// while (1)
	// {
		printf("salam\n");
	// }
	return (NULL);
}

// void	init_mutex(t_philo *philo)
// {
// 	int	i;

// 	i = 0;
// 	philo->forks = malloc(NBR_PHILO * sizeof(pthread_mutex_t));
// 	while (i < NBR_PHILO)
// 	{
// 		pthread_mutex_init(&philo->forks[i], NULL);
// 		i++;
// 	}
// }

// void	init_philo(t_philo *philo)
// {
// 	int	i;

// 	philo->t1 = malloc(NBR_PHILO * sizeof(t_philo));
// 	philo->ph = malloc(NBR_PHILO * sizeof(t_ph));
// 	i = 0;
// 	while (i < NBR_PHILO)
// 	{
// 		philo->ph[i].id = i + 1;
// 		philo-> ph[i].l_fork = &philo->forks[i];
// 		philo-> ph[i].r_fork = &philo->forks[(i + 1) % NBR_PHILO];
// 		pthread_create(&philo->t1[i], NULL, ss, NULL);
// 		i++;
// 	}
// }


void	init_mutex(t_help *help)
{
	int	i;

	i = 0;
	help->forks = malloc (NBR_PHILO * sizeof(pthread_mutex_t));
	while (i < NBR_PHILO)
	{
		pthread_mutex_init(&help->forks[i], NULL);
		i++;
	}
}

void	init_philo(t_help *help)
{
	int	i;

	i = 0;
	//init threads
	help->t1 = malloc (NBR_PHILO * sizeof(pthread_t));
	// init struct philo
	help->philo = malloc(NBR_PHILO * sizeof(t_philo));
	while (i < NBR_PHILO)
	{
		help->philo[i].id = i + 1;
		help->philo[i].l_fork = &help->forks[i];
		help->philo[i].l_fork = &help->forks[(i + 1) % NBR_PHILO];
		pthread_create(&help->t1[i], NULL, ss, NULL);
		i++;
	}
}

void	join_philo(t_help *help)
{
	int	i;

	i = 0;
	while (i < NBR_PHILO)
	{
		pthread_join(help->t1[i], NULL);
		i++;
	}
}

int	main()
{
	t_help	*help;

	help = malloc(sizeof(t_help));
	init_mutex(help);
	init_philo(help);
	join_philo(help);
	return (0);
}

// philo ==> thread
// forks ==> mutex