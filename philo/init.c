#include "philo.h"

static int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(t_fork) * data->num_philos);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
		{
			while (i-- > 0)
				pthread_mutex_destroy(&data->forks[i].mutex);
			free(data->forks);
			data->forks = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}

static int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = 0;
		data->philos[i].left_id = i;
		data->philos[i].right_id = (i + 1) % data->num_philos;
		data->philos[i].data = data;
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
		{
			while (i-- > 0)
				pthread_mutex_destroy(&data->philos[i].meal_mutex);
			free(data->philos);
			data->philos = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_data(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (0);
	if (!init_forks(data))
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (0);
	}
	if (!init_philos(data))
	{
		cleanup(data);
		return (0);
	}
	return (1);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (data->forks && i < data->num_philos)
		pthread_mutex_destroy(&data->forks[i++].mutex);
	free(data->forks);
	data->forks = NULL;
	i = 0;
	while (data->philos && i < data->num_philos)
		pthread_mutex_destroy(&data->philos[i++].meal_mutex);
	free(data->philos);
	data->philos = NULL;
	pthread_mutex_destroy(&data->print_mutex);
}
