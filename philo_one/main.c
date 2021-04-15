/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:44:51 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/15 11:23:33 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int ft_strlen(char *str){
	int i;

	i = 0;
	while (str && str[i] != '\0')
		i++;
	return (i);
}

int	print(char *str, int status)
{
	write(1, str, ft_strlen(str));
	return (status);
}

int	init_philos(t_state *state)
{
	int i;

	i = -1;
	while (++i < state->nb_philos)
	{
		state->philos[i].id = i + 1;
		state->philos[i].time_take_fork = 0;
		state->philos[i].time_die = 0;
		state->philos[i].time_eat = 0;
		state->philos[i].time_think = 0;
		state->philos[i].time_sleep = 0;
		state->philos[i].nb_time_eat = 0;
		state->philos[i].lfork = i;
		state->philos[i].rfork = (i - 1) % (state->nb_philos);
		state->philos[i].state = state;
	}
	return (0);
}
int	parse_input(t_state *state, int n, char **tab)
{
	state->nb_philos = ft_atoi(tab[1]);
	state->time_to_die = (long)ft_atoi(tab[2]) * 1000;
	state->time_to_eat = (long)ft_atoi(tab[3]) * 1000;
	state->time_to_sleep = (long)ft_atoi(tab[4]) * 1000;
	if (n == 6)
		state->nb_time_of_eat = ft_atoi(tab[5]);
	else
		state->nb_time_of_eat = -1;
	state->philos = (t_phil*)malloc(sizeof(t_phil) * state->nb_philos);
	state->tid = (pthread_t*)malloc(sizeof(pthread_t) * state->nb_philos);
	state->forks = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * state->nb_philos);
	state->writing = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	state->dieing = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	if (!state->philos || !state->forks)
		return (1);
	return (init_philos(state));
}

int	init_mutexes(t_state *state)
{
	int i;

	i = 0;
	while (i < state->nb_philos)
	{
		if(pthread_mutex_init(&(state->forks[i]), NULL) != 0)
			return (1);
		i++;
	}
	pthread_mutex_init(state->writing, NULL);
	return (0);
}

void	printer(long time, t_phil *philos, char *task, int died)
{
	//lock
	pthread_mutex_lock(philos->state->writing);
	ft_putnbr_fd(time, 1);
	write(1, " ", 1);
	ft_putnbr_fd(philos->id, 1);
	ft_putstr_fd(task, 1);
	if(died)
		exit(0);
	pthread_mutex_unlock(philos->state->writing);
	//unlock
}

// void	print_status(t_phil *philos)
// {
// 	lock
// 	int id = philos->id;
// 	printer(philos->time_take_fork, id, " has taken a fork\n");
// 	printer(philos->time_eat, id, " is eating\n");
// 	printer(philos->time_sleep, id, " is sleeping\n");
// 	printer(philos->time_think, id, " is thinking\n");
// 	printer(philos->time_die, id, " died\n");
// 	unlock
// }

// void detach_philosophers(t_phil *philos)
// {
// 	t_state *state = philos->state;

// 	int i = -1;
// 	while (++i < state->nb_philos)
// 		pthread_detach(state->philos[i].tid);
// 	i = -1;
// 	while(++i < state->nb_philos)
// 		pthread_mutex_destroy(&(state->forks[i]));
// }

long	get_time()
{
	struct timeval	*time;
	long t;
	time = (struct timeval*)malloc(sizeof(struct timeval));
	gettimeofday(time, 0);
	t = time->tv_sec * 1000 + time->tv_usec/1000;
	free(time);
	return (t);
}

void	eat(t_phil *philos)
{
	long time = get_time();
	pthread_mutex_lock(&(philos->state->forks[philos->lfork]));
	printer(get_time(), philos, " has taken a fork\n", 0);
	pthread_mutex_lock(&(philos->state->forks[philos->rfork]));
	printer(get_time(), philos, " has taken a fork\n", 0);
	philos->time_eat = get_time();
	printer(philos->time_eat, philos, " is eating\n", 0);
	usleep(philos->state->time_to_eat);
	pthread_mutex_unlock(&(philos->state->forks[philos->lfork]));
	pthread_mutex_unlock(&(philos->state->forks[philos->rfork]));
}


void	ssleep(t_phil *philos)
{
	philos->time_sleep = get_time();
	philos->time_die += philos->time_sleep - philos->time_eat;
	printer(philos->time_sleep, philos, " is sleeping\n", 0);
	usleep(philos->state->time_to_sleep);
}

void	think(t_phil *philos)
{
	philos->time_think = get_time();
	philos->time_die += philos->time_think - philos->time_sleep;
	printer(get_time(), philos, " is thinking\n", 0);
}

void	*start_routine(void *philos)
{
	t_phil *phil = (t_phil*)philos;
	while(1)
	{
		eat(phil);
		ssleep(phil);
		think(phil);
	}
	return NULL;
}

void	*verify_death(void *st)
{
	int i;
	t_state *state = (t_state*)st;
	i = -1;
	while(++i < state->nb_philos)
	{
		pthread_mutex_lock(state->dieing);
		if(state->philos[i].time_die >= state->time_to_die)
			printer(get_time(), &(state->philos[i]), " has died\n", 1);
		pthread_mutex_unlock(state->dieing);
	}
	return NULL;
}

int		create_threads(t_state *state)
{
	int		i;
	void	*philos;

	i = 0;
	while (i < state->nb_philos)
	{
		//printf("philos n: %d\n", state->philos[i].id);
		philos = (void*)&(state->philos[i]);
		if(pthread_create(&(state->philos[i].tid), NULL, &start_routine, philos) != 0 || pthread_create(&(state->tid[i]), NULL, &verify_death, (void*)state) != 0)
			return (1);
		i++;
	}
	return (0);
}

int		join_threads(t_state *state)
{
	int i;
	
	i = 0;
	while (i < state->nb_philos)
	{
		if(pthread_join(state->philos[i].tid, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}
int		run_simulation(t_state *state)
{
	if(init_mutexes(state))
		return (1);
	if(create_threads(state))
		return (1);
	if(join_threads(state))
		return (1);
	return (0);
}

int		main(int argc, char **argv)
{
	t_state	state;
	
	if (argc < 5 || argc > 6)
		return (print("Args count is not correct!\n", 1));
	if(parse_input(&state, argc, argv))
		return (print("Fatal Error, parsing!\n", 1));
	if(run_simulation(&state))
		return (print("Simulation failed!\n", 1));
	return (0);
}
