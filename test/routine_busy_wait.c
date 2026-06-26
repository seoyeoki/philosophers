#include "philo.h"

static void	take_forks(t_philo *philo)
{
  t_data *data;

  data = philo->data;
  if (philo->left_id < philo->right_id) {
    pthread_mutex_lock(&data->forks[philo->left_id].mutex);
    print_state(philo, "has taken a fork");
    pthread_mutex_lock(&data->forks[philo->right_id].mutex);
  } else {
    pthread_mutex_lock(&data->forks[philo->right_id].mutex);
    print_state(philo, "has taken a fork");
    pthread_mutex_lock(&data->forks[philo->left_id].mutex);
  }
  print_state(philo, "has taken a fork");
}

static void philo_eat(t_philo *philo) {
  t_data *data;

  data = philo->data;
  take_forks(philo);
  if (!is_dead(data)) {
    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal_time = get_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->meal_mutex);
    print_state(philo, "is eating");
    ft_usleep(data->time_to_eat);
  }
  pthread_mutex_unlock(&data->forks[philo->left_id].mutex);
  pthread_mutex_unlock(&data->forks[philo->right_id].mutex);
}

static void philo_sleep_think(t_philo *philo) {
  t_data *data;
  long long think_time;

  data = philo->data;
  print_state(philo, "is sleeping");
  ft_usleep(data->time_to_sleep);
  print_state(philo, "is thinking");
  if (data->num_philos % 2 == 1) {
    think_time = data->time_to_eat * 2 - data->time_to_sleep;
    if (think_time > 0)
      ft_usleep(think_time);
  }
}

static void single_philo(t_philo *philo) {
  t_data *data;

  data = philo->data;
  pthread_mutex_lock(&data->forks[0].mutex);
  print_state(philo, "has taken a fork");
  while (!is_dead(philo->data)) {
    // Busy wait loop (no sleep/usleep)
  }
  pthread_mutex_unlock(&data->forks[0].mutex);
}

void *philo_routine(void *arg) {
  t_philo *philo;

  philo = (t_philo *)arg;
  if (philo->data->num_philos == 1) {
    single_philo(philo);
    return (NULL);
  }
  if (philo->id % 2 == 0)
    ft_usleep(philo->data->time_to_eat);
  while (!is_dead(philo->data)) {
    philo_eat(philo);
    if (is_dead(philo->data))
      break;
    philo_sleep_think(philo);
  }
  return (NULL);
}
