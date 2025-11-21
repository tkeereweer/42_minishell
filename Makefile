SRCS = parsing/logic_tree.c \
	parsing/command_tree.c \
	parsing/build_node_list.c \
	parsing/parse_pipeline.c \
	parsing/parsing_utils.c \
	parsing/parsing_utils2.c \
	parsing/parsing_utils3.c \
	parsing/token_redir.c \
	parsing/token.c \
	parsing/token2.c \
	parsing/get_args.c \
	parsing/heredoc_function.c \
	parsing/heredoc_command.c \
	tree_visualiser.c \
	main.c

OBJS = $(SRCS:.c=.o)

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

LINKS = $(LIBFT) -lreadline

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINKS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make clean -C $(LIBFT_DIR)
	rm -rf $(OBJS)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re