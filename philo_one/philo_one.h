/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:34:08 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/15 10:52:51 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
#define PHILO_ONE_H

#include <pthread.h> 
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>

typedef struct		s_phil
{
	pthread_t		tid;
	int				id;
	int				lfork;
	int				rfork;
	long			time_take_fork;
	long			time_die;
	long			time_eat;
	long			time_think;
	long			time_sleep;
	int				nb_time_eat;
	struct s_state	*state;			
}					t_phil;

typedef struct		s_state
{
	t_phil			*philos;
	pthread_t		*tid;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*writing;
	pthread_mutex_t	*dieing;
	int				nb_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				nb_time_of_eat;
}					t_state;

int		ft_atoi(const char *str);
void	ft_putnbr_fd(long n, int fd);
void	ft_putstr_fd(char *s, int fd);

#endif
