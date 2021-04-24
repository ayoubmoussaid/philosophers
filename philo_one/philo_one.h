/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:34:08 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/24 14:18:26 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <pthread.h> 
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>
# include <stdio.h>

typedef struct s_phil
{
	pthread_mutex_t	mutex;
	int				id;
	int				is_eating;
	int				lfork;
	int				rfork;
	long			time_die;
	long			time_eat;
	int				nb_time_eat;
	struct s_state	*state;			
}					t_phil;

typedef struct s_state
{
	t_phil			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	writing;
	pthread_mutex_t	dieing;
	long			start;
	int				nb_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				nb_time_of_eat;
}					t_state;

int					ft_atoi(const char *str);
void				ft_putnbr_fd(long n, int fd);
void				ft_putstr_fd(char *s, int fd);
void				printer(t_phil *philos, char *task, int died);
int					print(char *str, int exitstatus);
int					ft_strlen(char *str);
void				eat(t_phil *philos);
void				ssleep(t_phil *philos);
long				get_time(void);
int					init_mutexes(t_state *state);
int					parse_input(t_state *state, int n, char **tab);
int					init_philos(t_state *state);
void				clear_state(t_state *state);
void				*verify_death(void *philos);

#endif
