/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:34:08 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/24 14:49:30 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <pthread.h> 
# include <signal.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>
# include <semaphore.h>

# define SEM_FORK	"SEM_FORK"
# define SEM_WRITE	"SEM_WRITE"
# define SEM_DEAD	"SEM_DEAD"
# define SEM_PHILO	"SEM_PHILO"
# define SEM_EAT	"SEM_EAT"

typedef struct s_phil
{
	sem_t			*mutex;
	sem_t			*eat_mutex;
	int				id;
	pid_t			pid;
	int				is_eating;
	int				died;
	long			time_die;
	long			time_eat;
	int				nb_time_eat;
	struct s_state	*state;			
}					t_phil;

typedef struct s_state
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
int		ft_strlen(char *str);
int		print(char *str, int exitstatus);
long	get_time(void);
void	printer(t_phil *philos, char *task, int died);
int		init_philos(t_state *state);
int		parse_input(t_state *state, int n, char **tab);
void	make_name(char *name, char *newname, int index);
int		init_mutexes(t_state *state);
int		eat(t_phil *philos);
int		ssleep(t_phil *philos);
void	*verify_end_of_eat(void *st);
void	*verify_death(void *philos);
#endif
