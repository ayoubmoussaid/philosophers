/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:34:08 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/23 08:33:10 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <pthread.h> 
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>
# include <semaphore.h>

# define SEM_FORK	"SEM_FORK"
# define SEM_WRITE	"SEM_WRITE"
# define SEM_DEAD	"SEM_DEAD"
# define SEM_PHILO	"SEM_PHILO"

typedef struct		s_phil
{
	sem_t			*mutex;
	int				id;
	int				is_eating;
	int				lfork;
	int				rfork;
	long			time_die;
	long			time_eat;
	int				nb_time_eat;
	struct s_state	*state;			
}					t_phil;

typedef struct		s_state
{
	t_phil			*philos;
	sem_t			*forks;
	sem_t			*writing;
	sem_t			*dieing;
	long			start;
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
