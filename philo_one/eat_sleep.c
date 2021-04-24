/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 13:48:22 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/24 15:46:22 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	eat(t_phil *philos)
{
	pthread_mutex_lock(&(philos->state->forks[philos->lfork]));
	printer(philos, " has taken a fork\n", 0);
	pthread_mutex_lock(&(philos->state->forks[philos->rfork]));
	printer(philos, " has taken a fork\n", 0);
	pthread_mutex_lock(&(philos->mutex));
	philos->is_eating = 1;
	philos->time_eat = get_time();
	philos->time_die = philos->time_eat + philos->state->time_to_die;
	printer(philos, " is eating\n", 0);
	usleep(philos->state->time_to_eat * 1000);
	philos->is_eating = 0;
	pthread_mutex_unlock(&(philos->mutex));
	pthread_mutex_unlock(&(philos->state->forks[philos->lfork]));
	pthread_mutex_unlock(&(philos->state->forks[philos->rfork]));
}

void	ssleep(t_phil *philos)
{
	printer(philos, " is sleeping\n", 0);
	usleep(philos->state->time_to_sleep * 1000);
}

void	*verify_death(void *philos)
{
	t_phil	*philo;

	philo = (t_phil *)philos;
	while (1)
	{
		pthread_mutex_lock(&(philo->mutex));
		if (!philo->is_eating && get_time() > philo->time_die)
			printer(philo, " has died\n", 1);
		pthread_mutex_unlock(&(philo->mutex));
		usleep(1000);
	}
	return ((void *)0);
}
