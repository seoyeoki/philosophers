#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	int				left_fork_id;
	int				right_fork_id;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
}	t_philo;

struct s_data
{
	int				num_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				must_eat;
	long long		start_time;
	int				over;
	pthread_mutex_t	print_mutex;
	t_fork			*forks;
	t_philo			*philos;
};

int			init_dinner(t_data *data);
void		cleanup(t_data *data);
long long	get_time(void);
void		ft_usleep(long long ms);
int			is_over(t_data *data);
int			print_state(t_philo *philo, const char *state);
void		*dining_routine(void *arg);
void		*monitor_dinner(void *arg);
int			start_dinner(t_data *data);

#endif
