/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 13:50:18 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/24 16:13:38 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	init_philos(t_state *state)
{
	int	i;

	i = -1;
	while (++i < state->nb_philos)
	{
		state->philos[i].id = i + 1;
		state->philos[i].is_eating = 0;
		state->philos[i].time_die = 0;
		state->philos[i].time_eat = 0;
		state->philos[i].nb_time_eat = 0;
		state->philos[i].lfork = i;
		state->philos[i].rfork = (i + 1) % (state->nb_philos);
		state->philos[i].state = state;
	}
	return (0);
}

int	parse_input(t_state *state, int n, char **tab)
{
	state->nb_philos = ft_atoi(tab[1]);
	state->time_to_die = (long)ft_atoi(tab[2]);
	state->time_to_eat = (long)ft_atoi(tab[3]);
	state->time_to_sleep = (long)ft_atoi(tab[4]);
	if (n == 6)
		state->nb_time_of_eat = ft_atoi(tab[5]);
	else
		state->nb_time_of_eat = -1;
	state->philos = (t_phil *)malloc(sizeof(t_phil) * state->nb_philos);
	state->forks = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t) * state->nb_philos);
	if (!state->philos || !state->forks)
		return (1);
	return (init_philos(state));
}

int	init_mutexes(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->nb_philos)
	{
		if (pthread_mutex_init(&(state->forks[i]), NULL) != 0)
			return (1);
		if (pthread_mutex_init(&(state->philos[i].mutex), NULL) != 0)
			return (1);
		i++;
	}
	pthread_mutex_init(&(state->writing), NULL);
	pthread_mutex_init(&(state->dieing), NULL);
	pthread_mutex_lock(&(state->dieing));
	return (0);
}

void	clear_state(t_state *state)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&state->writing);
	pthread_mutex_destroy(&state->dieing);
	while (++i < state->nb_philos)
	{
		pthread_mutex_destroy(&state->forks[i]);
		pthread_mutex_destroy(&state->philos[i].mutex);
	}
	free(state->forks);
	free(state->philos);
}
