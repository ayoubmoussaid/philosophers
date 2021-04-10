# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amoussai <amoussai@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/09/10 15:30:39 by amoussai          #+#    #+#              #
#    Updated: 2021/04/10 12:12:51 by amoussai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=philosphers
LIBFT=libft.a
SRCS=	main.c utils.c

		
OBJ=$(SRCS:.c=.o)
FLAGS=-Wall -Werror -Wextra
CC=gcc

.PHONY: clean bonus fclean all re

all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) -g $(SRCS) -o $(NAME)

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all
