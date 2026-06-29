#include "philo.h"

static int	check_death(t_data *data)
{
	int			i;
	long long	time_since_meal;
	long long	timestamp;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		time_since_meal = get_time() - data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		if (time_since_meal > data->time_to_die)
		{
			pthread_mutex_lock(&data->print_mutex);
			if (!data->over)
			{
				timestamp = get_time() - data->start_time;
				printf("%lld %d died\n", timestamp, data->philos[i].id);
				data->over = 1;
			}
			pthread_mutex_unlock(&data->print_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_all_ate(t_data *data)
{
	int	i;
	int	count;

	if (data->must_eat < 0)
		return (0);
	i = 0;
	count = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].meals_eaten >= data->must_eat)
			count++;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		i++;
	}
	if (count == data->num_philos)
	{
		pthread_mutex_lock(&data->print_mutex);
		data->over = 1;
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_dinner(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_death(data) || check_all_ate(data))
			break ;
		usleep(1000);
	}
	return (NULL);
}

int	start_dinner(t_data *data)
{
	pthread_t	monitor;
	int			i;

	data->start_time = get_time();
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL, dining_routine,
				&data->philos[i]) != 0)
			return (0);
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_dinner, data) != 0)
		return (0);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	return (1);
}
