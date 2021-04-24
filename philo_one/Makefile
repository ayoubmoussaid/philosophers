# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/09/10 15:30:39 by amoussai          #+#    #+#              #
#    Updated: 2021/04/24 13:58:52 by amoussai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=philo_one
LIBFT=libft.a
SRCS=main.c utils.c eat_sleep.c init.c messages.c

		
OBJ=$(SRCS:.c=.o)
FLAGS=-Wall -Werror -Wextra 
CC=gcc

.PHONY: clean bonus fclean all re

all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) -g $(SRCS) -o $(NAME) -lpthread

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all
