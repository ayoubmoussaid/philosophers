/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:34:08 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/24 15:11:36 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

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

typedef struct s_phil
{
	sem_t			*mutex;
	int				id;
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
void	printer(t_phil *philos, char *task, int died);
long	get_time(void);
int		print(char *str, int exitstatus);
int		ft_strlen(char *str);
int		init_philos(t_state *state);
int		parse_input(t_state *state, int n, char **tab);
void	make_name(char *name, char *newname, int index);
int		init_mutexes(t_state *state);
int		eat(t_phil *philos);
int		ssleep(t_phil *philos);
void	*verify_death(void *philos);
void	clear_state(t_state *state);
#endif
