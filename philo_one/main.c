/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:44:51 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/24 14:13:00 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	routine(t_phil *phil)
{
	eat(phil);
	ssleep(phil);
	printer(phil, " is thinking\n", 0);
}

void	*start_routine(void *philos)
{
	t_phil		*phil;
	pthread_t	tid;
	static int	nb = 0;

	phil = (t_phil *)philos;
	phil->time_die = get_time() + phil->state->time_to_die;
	if (pthread_create(&tid, NULL, &verify_death, philos) != 0)
		return ((void *)0);
	pthread_detach(tid);
	while (1)
	{
		if (phil->state->nb_time_of_eat != -1
			&& phil->nb_time_eat >= phil->state->nb_time_of_eat)
		{
			pthread_mutex_lock(&(phil->mutex));
			nb++;
			if (nb == phil->state->nb_philos)
				pthread_mutex_unlock(&(phil->state->dieing));
			return ((void *)0);
		}
		routine(phil);
		phil->nb_time_eat++;
	}
	return ((void *)0);
}

int	create_threads(t_state *state)
{
	int			i;
	pthread_t	tid;
	void		*philos;

	state->start = get_time();
	i = 0;
	while (i < state->nb_philos)
	{
		philos = (void *)(&state->philos[i]);
		if (pthread_create(&tid, NULL, &start_routine, philos) != 0)
			return (1);
		pthread_detach(tid);
		usleep(100);
		i++;
	}
	return (0);
}

int	run_simulation(t_state *state)
{
	if (init_mutexes(state))
		return (1);
	if (create_threads(state))
		return (1);
	pthread_mutex_lock(&(state->dieing));
	pthread_mutex_unlock(&(state->dieing));
	return (0);
}

int	main(int argc, char **argv)
{
	t_state	state;

	if (argc < 5 || argc > 6)
		return (print("Args count is not correct!\n", 1));
	if (parse_input(&state, argc, argv))
		return (print("Fatal Error, parsing!\n", 1));
	if (run_simulation(&state))
		return (print("Simulation failed!\n", 1));
	clear_state(&state);
	return (0);
}
