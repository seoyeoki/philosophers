#include "philo.h"

static void	take_forks(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_lock(&data->forks[philo->left_fork_id].mutex);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&data->forks[philo->right_fork_id].mutex);
	}
	else
	{
		pthread_mutex_lock(&data->forks[philo->right_fork_id].mutex);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&data->forks[philo->left_fork_id].mutex);
	}
	print_state(philo, "has taken a fork");
}

static void	philo_eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	take_forks(philo);
	if (!is_over(data))
	{
		pthread_mutex_lock(&philo->meal_mutex);
		philo->last_meal_time = get_time();
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->meal_mutex);
		print_state(philo, "is eating");
		ft_usleep(data->time_to_eat);
	}
	pthread_mutex_unlock(&data->forks[philo->left_fork_id].mutex);
	pthread_mutex_unlock(&data->forks[philo->right_fork_id].mutex);
}

static void	philo_sleep_think(t_philo *philo)
{
	t_data		*data;
	long long	think_time;

	data = philo->data;
	print_state(philo, "is sleeping");
	ft_usleep(data->time_to_sleep);
	print_state(philo, "is thinking");
	if (data->num_philos % 2 == 1)
	{
		think_time = (data->time_to_eat * 3) / 2 - data->time_to_sleep;
		if (think_time > 0)
			ft_usleep(think_time);
	}
}

static void	init_delay(t_philo *philo)
{
	int			delay_mult;
	long long	t_cycle;
	long long	think;

	if (philo->data->num_philos % 2 == 0 && philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat);
	else if (philo->data->num_philos % 2 == 1)
	{
		delay_mult = ((philo->id - 1) * 2) % 5;
		if (philo->id == philo->data->num_philos && delay_mult == 0)
			delay_mult = 2;
		if (delay_mult > 0)
		{
			think = (philo->data->time_to_eat * 3) / 2
				- philo->data->time_to_sleep;
			if (think < 0)
				think = 0;
			t_cycle = philo->data->time_to_eat
				+ philo->data->time_to_sleep + think;
			ft_usleep((t_cycle * delay_mult) / 5);
		}
	}
}

void	*dining_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(&philo->data->forks[0].mutex);
		print_state(philo, "has taken a fork");
		ft_usleep(philo->data->time_to_die + 1);
		pthread_mutex_unlock(&philo->data->forks[0].mutex);
		return (NULL);
	}
	init_delay(philo);
	while (!is_over(philo->data))
	{
		philo_eat(philo);
		if (is_over(philo->data))
			break ;
		philo_sleep_think(philo);
	}
	return (NULL);
}
