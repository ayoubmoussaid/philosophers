/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 14:57:24 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/24 15:10:06 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] != '\0')
		i++;
	return (i);
}

int	print(char *str, int exitstatus)
{
	write(1, str, ft_strlen(str));
	return (exitstatus);
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, 0);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	printer(t_phil *philos, char *task, int died)
{
	static int	done = 0;

	sem_wait(philos->state->writing);
	if (!done)
	{
		ft_putnbr_fd(get_time() - philos->state->start, 1);
		write(1, " ", 1);
		ft_putnbr_fd(philos->id, 1);
		write(1, task, ft_strlen(task));
		if (died)
		{
			sem_post(philos->state->dieing);
			done = 1;
		}
	}
	sem_post(philos->state->writing);
}
