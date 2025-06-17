# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/18 12:05:24 by snazzal           #+#    #+#              #
#    Updated: 2025/02/18 13:51:32 by snazzal          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c
OBJS = ${SRCS:.c=.o}
NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f

all: ${NAME}
${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}
clean:
	${RM} ${OBJS}
fclean: clean
	${RM} ${NAME}
re: fclean all
.PHONY: all clean fclean re
