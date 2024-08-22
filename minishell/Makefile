NAME	= minishell
CC	= cc
CFLAGS	= -Wall -Werror -Wextra -g -Iincs -I$(LIBFTDIR)
LFLAGS	= -L$(LIBFTDIR) -lreadline -lft
SRCS	= $(shell find . -name "*.c" | grep -v -e '$(LIBFTDIR)')
OBJ_DIR	= objs
OBJS	= $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
LIBFTDIR= incs/libft
LIBFT	= $(LIBFTDIR)/libft.a
VPATH	= $(sort $(dir $(SRCS)))

all:	$(NAME)

$(NAME):$(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)

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

.PHONY:	all clean fclean re
