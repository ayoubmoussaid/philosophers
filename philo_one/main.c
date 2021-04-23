/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:44:51 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/23 08:17:03 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		ft_strlen(char *str){
	int i;

	i = 0;
	while (str && str[i] != '\0')
		i++;
	return (i);
}

int		print(char *str, int exitstatus)
{
	write(1, str, ft_strlen(str));
	return (exitstatus);
}

int		init_philos(t_state *state)
{
	int i;

	i = -1;
	while (++i < state->nb_philos)
	{
		state->philos[i].id = i + 1;
		state->philos[0].is_eating = 0;
		state->philos[i].time_die = 0;
		state->philos[i].time_eat = 0;
		state->philos[i].nb_time_eat = 0;
		state->philos[i].lfork = i;
		state->philos[i].rfork = (i - 1) % (state->nb_philos);
		state->philos[i].state = state;
	}
	return (0);
}

int		parse_input(t_state *state, int n, char **tab)
{
	state->nb_philos = ft_atoi(tab[1]);
	state->time_to_die = (long)ft_atoi(tab[2]);
	state->time_to_eat = (long)ft_atoi(tab[3]);
	state->time_to_sleep = (long)ft_atoi(tab[4]);
	if (n == 6)
		state->nb_time_of_eat = ft_atoi(tab[5]);
	else
		state->nb_time_of_eat = -1;
	state->philos = (t_phil*)malloc(sizeof(t_phil) * state->nb_philos);
	state->forks = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * state->nb_philos);
	if (!state->philos || !state->forks)
		return (1);
	return (init_philos(state));
}

int		init_mutexes(t_state *state)
{
	int i;

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

long	get_time()
{
	struct timeval	time;

	gettimeofday(&time, 0);
	return (time.tv_sec * 1000 + time.tv_usec/1000);
}

void	printer(t_phil *philos, char *task, int died)
{
	static int done = 0;
	
	pthread_mutex_lock(&(philos->state->writing));
	if (!done)
	{
		ft_putnbr_fd(get_time() - philos->state->start, 1);
		write(1, " ", 1);
		ft_putnbr_fd(philos->id, 1);
		write(1, task, ft_strlen(task));
		if(died)
		{
			pthread_mutex_unlock(&(philos->state->dieing));
			done = 1;
		}
	}
	
	pthread_mutex_unlock(&(philos->state->writing));
}

void	eat(t_phil *philos)
{
	pthread_mutex_lock(&(philos->state->forks[philos->rfork]));
	printer(philos, " has taken a fork\n", 0);
	pthread_mutex_lock(&(philos->state->forks[philos->lfork]));
	printer(philos, " has taken a fork\n", 0);
	pthread_mutex_lock(&(philos->mutex));
	philos->is_eating = 1;
	philos->time_eat = get_time();
	philos->time_die = philos->time_eat + philos->state->time_to_die;
	printer(philos, " is eating\n", 0);
	usleep(philos->state->time_to_eat * 1000);
	philos->is_eating = 0;
	pthread_mutex_unlock(&(philos->mutex));
}


void	ssleep(t_phil *philos)
{
	printer(philos, " is sleeping\n", 0);
	pthread_mutex_unlock(&(philos->state->forks[philos->lfork]));
	pthread_mutex_unlock(&(philos->state->forks[philos->rfork]));
	usleep(philos->state->time_to_sleep * 1000);
}


void	*verify_death(void *philos)
{
	t_phil	*philo;

	philo = (t_phil*)philos;
	while (1)
	{
		pthread_mutex_lock(&(philo->mutex));
		if(!philo->is_eating && get_time() > philo->time_die)
			printer(philo, " has died\n", 1);
		pthread_mutex_unlock(&(philo->mutex));
		usleep(1000);
	}
	return ((void*)0);
}

void	*start_routine(void *philos)
{
	t_phil		*phil;
	pthread_t	tid;
	static int	nb = 0;
	phil = (t_phil*)philos;
	phil->time_die = get_time() + phil->state->time_to_die;
	if (pthread_create(&tid, NULL, &verify_death, philos) != 0)
		return ((void*)0);
	pthread_detach(tid);
	while (1)
	{
		if(phil->state->nb_time_of_eat != -1 && phil->nb_time_eat >= phil->state->nb_time_of_eat)
		{
			pthread_mutex_lock(&(phil->mutex));
			nb++;
			if(nb == phil->state->nb_philos)
				pthread_mutex_unlock(&(phil->state->dieing));
			return ((void*)0);
		}
		eat(phil);
		ssleep(phil);
		printer(philos, " is thinking\n", 0);
		phil->nb_time_eat++;
	}
	return((void*)0);;
}


int		create_threads(t_state *state)
{
	int			i;
	pthread_t	tid;
	void		*philos;

	state->start = get_time();
	i = 0;
	while (i < state->nb_philos)
	{
		philos = (void*)(&state->philos[i]);
		if(pthread_create(&tid, NULL, &start_routine, philos) != 0)
			return (1);
		pthread_detach(tid);
		usleep(100);
		i++;
	}
	return (0);
}

int		run_simulation(t_state *state)
{
	if (init_mutexes(state))
		return (1);
	if (create_threads(state))
		return (1);
	pthread_mutex_lock(&(state->dieing));
	pthread_mutex_unlock(&(state->dieing));
	return (0);
}

void	clear_state(t_state *state)
{
	int i;

	i = -1;
	pthread_mutex_destroy(&state->writing);
	pthread_mutex_destroy(&state->dieing);
	while(++i < state->nb_philos)
	{
		pthread_mutex_destroy(&state->forks[i]);	
		pthread_mutex_destroy(&state->philos[i].mutex);
	}
	free(state->philos);
}

int		main(int argc, char **argv)
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
