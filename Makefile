SRCS = parsing/logic_tree.c \
	parsing/command_tree.c \
	parsing/build_node_list.c \
	parsing/parse_pipeline.c \
	parsing/parsing_utils.c \
	parsing/parsing_utils2.c \
	parsing/parsing_utils3.c \
	parsing/parsing_utils4.c \
	parsing/parsing_utils5.c \
	parsing/token_redir.c \
	parsing/token.c \
	parsing/token2.c \
	parsing/variable_expansion/env_vars.c \
	parsing/variable_expansion/wildcards.c \
	parsing/variable_expansion/expand_vars.c \
	execution/builtins/ft_export.c \
	execution/builtins/ft_unset.c \
	execution/builtins/ft_env.c \
	execution/builtins/ft_cd.c \
	execution/builtins/ft_pwd.c \
	execution/builtins/ft_exit.c \
	execution/builtins/ft_echo.c \
	execution/builtins/run_builtins.c \
	execution/signals_parent.c \
	execution/signals_child.c \
	parsing/get_args.c \
	parsing/heredoc_function.c \
	parsing/heredoc_command.c \
	execution/exec_tree.c \
	execution/exec_cmd.c \
	execution/exec_cmd2.c \
	execution/create_pipe_and_fork.c \
	execution/int_tab_realloc.c \
	tree_visualiser.c \
	main.c

OBJS = $(SRCS:.c=.o)

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

LINKS = $(LIBFT) -L/opt/homebrew/opt/readline/lib -lreadline

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINKS) -o $(NAME)
	dsymutil $@

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c -I/opt/homebrew/opt/readline/include $< -o $@

clean:
	make clean -C $(LIBFT_DIR)
	rm -rf $(OBJS)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re