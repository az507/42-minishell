# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: achak <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/31 14:40:12 by achak             #+#    #+#              #
#    Updated: 2024/04/18 13:27:06 by achak            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CC1	= cc -g -Wall -Werror -Wextra
CC2	= cc -g -Wall -Werror -Wextra -fsanitize=memory
#CC2	= cc -g -Wall -Werror -Wextra -fsanitize=address -fno-omit-frame-pointer \
	  -fno-optimize-sibling-calls
SRCS	= $(wildcard *.c)

all:	$(NAME)

$(NAME):$(SRCS)
	$(CC1) $(SRCS) -o $(NAME) -lreadline

test1:
	make re && clear && valgrind ./$(NAME)

test2:
	make re && clear && ./$(NAME)

clean:
ifneq ("$(wildcard $(NAME))","")
	rm $(NAME)
endif

fclean:	clean

re:	fclean all

.PHONY:	all clean fclean re
