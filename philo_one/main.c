/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:44:51 by amoussai          #+#    #+#             */
/*   Updated: 2021/03/21 19:03:58 by amoussai         ###   ########.fr       */
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
	state->time_to_die = (long)ft_atoi(tab[2]);
	state->time_to_eat = (long)ft_atoi(tab[3]);
	state->time_to_sleep = (long)ft_atoi(tab[4]);
	if (n == 6)
		state->nb_time_of_eat = ft_atoi(tab[5]);
	else
		state->nb_time_of_eat = -1;
	state->philos = (t_phil*)malloc(sizeof(t_phil) * state->nb_philos);
	state->forks = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * state->nb_philos);
	state->writing = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
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

void	printer(int time, int id, char *task)
{
	//lock
	ft_putnbr_fd(time, 1);
	write(1, " ", 1);
	ft_putnbr_fd(id, 1);
	ft_putstr_fd(task, 1);
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

long	get_time()
{
	struct timeval	*time;

	gettimeofday(time, 0);
	return (time->tv_sec * 1000 + time->tv_usec);
}

void	eat(t_phil *philos)
{
	long time;

	printer(get_time(), philos->id, " has taken a fork\n");
	pthread_mutex_lock(&(philos->state->forks[philos->lfork]));
	pthread_mutex_lock(&(philos->state->forks[philos->rfork]));
	time = get_time();
	usleep(philos->state->time_to_eat);
	philos->time_eat += get_time() - time;
	pthread_mutex_unlock(&(philos->state->forks[philos->lfork]));
	pthread_mutex_unlock(&(philos->state->forks[philos->rfork]));
}


void	*start_routine(t_phil *philos)
{
	while(1)
	{
		eat(philos);
		// sleep();
		// think();
	}
}

int		create_threads(t_state *state)
{
	int		i;
	void	*philos;

	i = 0;
	while (i < state->nb_philos)
	{
		philos = (void*)&(state->philos[0]);
		if(pthread_create(&(state->philos[i].tid), NULL, &start_routine, philos) != 0)
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
		if(pthread_join(&(state->philos[i].tid), NULL) != 0)
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
