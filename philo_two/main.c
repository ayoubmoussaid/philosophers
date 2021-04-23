/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:44:51 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/23 16:23:57 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"
int g_max_eat_count = 1;

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
	sem_unlink(SEM_FORK);
	state->forks = sem_open(SEM_FORK, O_CREAT | O_EXCL, 0644, state->nb_philos);
	if (!state->philos || !state->forks)
		return (1);
	return (init_philos(state));}

void	make_name(char *name, char *newname, int index)
{
	int i;

	i = -1;
	while(name[++i] != '\0')
		newname[i] = name[i];
	newname[i] = index + '0';
	newname[i] = '\0';
}

int		init_mutexes(t_state *state)
{
	int		i;
	char	semname[300];

	i = 0;
	while (i < state->nb_philos)
	{
		make_name(SEM_PHILO, (char*)semname, i);
		sem_unlink(semname);
		state->philos[i].mutex = sem_open(semname, O_CREAT | O_EXCL, 0644, 1);
		i++;
	}
	sem_unlink(SEM_DEAD);
	sem_unlink(SEM_WRITE);
	state->dieing = sem_open(SEM_DEAD, O_CREAT | O_EXCL, 0644, 0);
	state->writing = sem_open(SEM_WRITE, O_CREAT | O_EXCL, 0644, 1);
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
	
	sem_wait(philos->state->writing);
	if (!done)
	{
		ft_putnbr_fd(get_time() - philos->state->start, 1);
		write(1, " ", 1);
		ft_putnbr_fd(philos->id, 1);
		write(1, task, ft_strlen(task));
		if(died)
		{
			sem_post(philos->state->dieing);
			done = 1;
		}
	}
	sem_post(philos->state->writing);
}

int		eat(t_phil *philos)
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

	philo = (t_phil*)philos;
	while (1)
	{
		sem_wait(philo->mutex);
		if(!philo->is_eating && get_time() > philo->time_die)
			printer(philo, " has died\n", 1);
		sem_post(philo->mutex);
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
			sem_wait(phil->mutex);
			nb++;
			if (nb == phil->state->nb_philos)
			{
				sem_post(phil->state->dieing);
			}	
			return ((void*)0);
		}
		if (eat(phil))
			return ((void*)0);
		if (ssleep(phil))
			return ((void*)0);
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
	sem_wait(state->dieing);
	write(1, "check\n", 6);
	return (0);
}

void	clear_state(t_state *state)
{
	int i;

	i = -1;
	sem_close(state->writing);
	sem_close(state->dieing);
	sem_close(state->forks);	
	while(++i < state->nb_philos)
	{
		write(1, "hehe\n", 5);
		sem_close(state->philos[i].mutex);
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
