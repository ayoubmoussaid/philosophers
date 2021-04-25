/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 14:38:06 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/25 07:34:06 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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
	if (state->nb_philos < 2 || state->nb_philos > 200
		|| state->time_to_die < 60 || state->time_to_eat < 60
		|| state->time_to_sleep < 60 || (n == 6 && state->nb_time_of_eat < 1))
		return (1);
	state->philos = (t_phil *)malloc(sizeof(t_phil) * state->nb_philos);
	sem_unlink(SEM_FORK);
	state->forks
		= sem_open(SEM_FORK, O_CREAT | O_EXCL, 0644, state->nb_philos);
	if (!state->philos || !state->forks)
		return (1);
	return (init_philos(state));
}

void	make_name(char *name, char *newname, int index)
{
	int	i;

	i = -1;
	while (name[++i] != '\0')
		newname[i] = name[i];
	newname[i] = index + '0';
	newname[i] = '\0';
}

int	init_mutexes(t_state *state)
{
	int		i;
	char	semname[300];

	i = 0;
	while (i < state->nb_philos)
	{
		make_name(SEM_PHILO, (char *)semname, i);
		sem_unlink(semname);
		state->philos[i].mutex = sem_open(semname, O_CREAT | O_EXCL, 0644, 1);
		make_name(SEM_EAT, (char *)semname, i);
		sem_unlink(semname);
		state->philos[i].eat_mutex
			= sem_open(semname, O_CREAT | O_EXCL, 0644, 0);
		i++;
	}
	sem_unlink(SEM_DEAD);
	sem_unlink(SEM_WRITE);
	state->dieing = sem_open(SEM_DEAD, O_CREAT | O_EXCL, 0644, 0);
	state->writing = sem_open(SEM_WRITE, O_CREAT | O_EXCL, 0644, 1);
	return (0);
}
