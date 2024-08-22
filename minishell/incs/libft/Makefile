# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: achak <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/13 15:53:42 by achak             #+#    #+#              #
#    Updated: 2024/08/22 19:39:07 by achak            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
INCLUDE = libft.h
FLAGS = -g -Wall -Wextra -Werror
#OTHERS = $(shell ls -l | grep '^d' | awk '{print $NF}')
AR = ar rc
MAKE = make
SRCS =  ft_isalnum.c ft_isprint.c ft_memcmp.c  ft_putchar_fd.c \
	ft_strlcat.c ft_strncmp.c ft_substr.c ft_isalpha.c \
	ft_memcpy.c  ft_putendl_fd.c ft_strchr.c  ft_strlcpy.c \
	ft_strnstr.c ft_tolower.c ft_bzero.c   ft_isascii.c ft_memcpy.c \
	ft_memmove.c ft_putnbr_fd.c  ft_strdup.c  ft_strlen.c  ft_strrchr.c \
	ft_toupper.c ft_calloc.c  ft_isdigit.c ft_memchr.c  \
	ft_putstr_fd.c  ft_strjoin.c ft_strmapi.c ft_strtrim.c

OBJ =  $(SRCS:.c=.o)

all:	$(NAME)

$(NAME):$(OBJ)
	$(AR)  $(NAME) $(OBJ)
	ranlib $(NAME)

#$(CC)  $(FLAGS) $(SRCS)
%.o:%.c
	$(CC) -c $(FLAGS) $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
