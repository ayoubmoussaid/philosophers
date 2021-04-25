/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 10:30:56 by amoussai          #+#    #+#             */
/*   Updated: 2021/04/24 14:49:53 by amoussai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

char	*ft_isspace(char *str)
{
	while ((*str) != '\0')
	{
		if ((*str) == '\t' || (*str) == '\n' || (*str) == '\v'
			|| (*str) == '\f' || (*str) == '\r' || (*str) == ' ')
			str++;
		else
			return (str);
	}
	return (str);
}

int	ft_atoi(const char *str)
{
	int	res;
	int	neg;
	int	x;

	str = ft_isspace((char *)str);
	neg = 1;
	res = 0;
	x = 0;
	if ((*str) == '-')
		neg = -1;
	if ((*str) == '+' || (*str) == '-')
		str++;
	while (((*str) >= '0' && (*str) <= '9') && (*str) != '\0')
	{
		res = res * 10;
		res += *str - '0';
		if (++x > 19 && neg == 1)
			return (-1);
		else if (x > 19 && neg == -1)
			return (0);
		str++;
	}
	return (res * neg);
}

void	ft_putnbr_fd(long n, int fd)
{
	long	reste;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n = -n;
	}
	if (n / 10 == 0)
		ft_putchar_fd((n % 10) + 48, fd);
	if (n / 10 > 0)
	{
		reste = n % 10;
		ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd(reste + 48, fd);
	}
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s == NULL)
		return ;
	while (*s)
		ft_putchar_fd(*s++, fd);
}
