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
CC	= cc
CFLAGS	= -Wall -Werror -Wextra -Iincs -I$(LIBFTDIR)
LFLAGS	= -L$(LIBFTDIR)
LDFLAGS = -lreadline -lft
SRCS	= $(shell find srcs -type f -name "*.c")
OBJ_DIR	= objs
OBJS	= $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
LIBFTDIR= incs/libft
LIBFT	= $(LIBFTDIR)/libft.a
VPATH	= $(sort $(dir $(SRCS)))

all:	$(NAME)

$(NAME):$(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS) $(LDFLAGS) $(XTRAFLAGS)

$(LIBFT):
	make -C$(LIBFTDIR)

$(OBJ_DIR)/%.o:%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make -C$(LIBFTDIR) clean

fclean:	clean
	rm -f $(NAME)
	make -C$(LIBFTDIR) fclean

re:	fclean all

docker:
	docker build -t minishell .
ifeq ($(shell uname),Linux)
	docker run --rm -it --name minishell -v '/:/hostfs:ro' minishell
else ifeq ($(shell uname),Darwin)
	docker run --rm -it --name minishell -v '/Users:/hostfs:ro' minishell
endif

docker-clean:
	-docker kill minishell
	-docker rmi minishell

.PHONY:	all clean fclean re docker docker-clean
