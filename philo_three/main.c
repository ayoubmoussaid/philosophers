/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:44:51 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/24 14:44:50 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	*start_routine(t_phil *phil)
{
	pthread_t	tid;

	phil->time_die = get_time() + phil->state->time_to_die;
	if (pthread_create(&tid, NULL, &verify_death, (void *)phil) != 0)
		return ((void *)0);
	pthread_detach(tid);
	while (1)
	{
		if (eat(phil))
			return ((void *)0);
		if (ssleep(phil))
			return ((void *)0);
		printer(phil, " is thinking\n", 0);
		phil->nb_time_eat++;
	}
	return ((void *)0);
}

int	create_threads(t_state *state)
{
	int			i;
	pthread_t	tid;

	state->start = get_time();
	if (state->nb_time_of_eat > 0)
	{
		if (pthread_create(&tid, NULL, &verify_end_of_eat, (void *)state) != 0)
			return (0);
		pthread_detach(tid);
	}
	i = 0;
	while (i < state->nb_philos)
	{
		state->philos[i].pid = fork();
		if (state->philos[i].pid == 0)
		{
			start_routine(&state->philos[i]);
			exit(0);
		}
		usleep(100);
		i++;
	}
	return (0);
}

int	run_simulation(t_state *state)
{
	int	i;

	if (init_mutexes(state))
		return (1);
	if (create_threads(state))
		return (1);
	sem_wait(state->dieing);
	i = -1;
	while (++i < state->nb_philos)
		kill(state->philos[i].pid, SIGKILL);
	return (0);
}

void	clear_state(t_state *state)
{
	int		i;
	char	semname[300];

	i = -1;
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_DEAD);
	sem_unlink(SEM_WRITE);
	while (++i < state->nb_philos)
	{
		make_name(SEM_PHILO, (char *)semname, i);
		sem_unlink(semname);
	}
	free(state->philos);
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
