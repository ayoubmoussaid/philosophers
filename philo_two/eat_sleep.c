/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 15:03:51 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/24 15:05:02 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int	eat(t_phil *philos)
{
	if (sem_wait(philos->state->forks) != 0)
		return (1);
	printer(philos, " has taken a fork\n", 0);
	if (sem_wait(philos->state->forks) != 0)
		return (1);
	printer(philos, " has taken a fork\n", 0);
	if (sem_wait(philos->mutex) != 0)
		return (1);
	philos->is_eating = 1;
	philos->time_eat = get_time();
	philos->time_die = philos->time_eat + philos->state->time_to_die;
	printer(philos, " is eating\n", 0);
	usleep(philos->state->time_to_eat * 1000);
	philos->is_eating = 0;
	if (sem_post(philos->mutex) != 0)
		return (1);
	return (0);
}

int	ssleep(t_phil *philos)
{
	printer(philos, " is sleeping\n", 0);
	if (sem_post(philos->state->forks) != 0)
		return (1);
	if (sem_post(philos->state->forks) != 0)
		return (1);
	usleep(philos->state->time_to_sleep * 1000);
	return (0);
}

void	*verify_death(void *philos)
{
	t_phil	*philo;

	philo = (t_phil *)philos;
	while (1)
	{
		sem_wait(philo->mutex);
		if (!philo->is_eating && get_time() > philo->time_die)
			printer(philo, " has died\n", 1);
		sem_post(philo->mutex);
		usleep(1000);
	}
	return ((void *)0);
}
