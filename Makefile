SRCS = parsing/logic_tree.c \
	parsing/logic_tree_utils.c \
	parsing/command_tree.c \
	parsing/build_node_list.c \
	parsing/parse_pipeline.c \
	parsing/parsing_utils.c \
	parsing/parsing_utils2.c \
	parsing/parsing_utils3.c \
	parsing/parsing_utils4.c \
	parsing/parsing_utils5.c \
	parsing/parsing_utils6.c \
	parsing/parsing_utils7.c \
	parsing/token_redir.c \
	parsing/token.c \
	parsing/token2.c \
	parsing/variable_expansion/env_vars.c \
	parsing/variable_expansion/wildcards.c \
	parsing/variable_expansion/expand_vars.c \
	parsing/variable_expansion/expansion_utils.c \
	parsing/variable_expansion/expansion_utils2.c \
	execution/builtins/ft_export.c \
	execution/builtins/ft_unset.c \
	execution/builtins/ft_env.c \
	execution/builtins/ft_cd.c \
	execution/builtins/ft_pwd.c \
	execution/builtins/ft_exit.c \
	execution/builtins/ft_echo.c \
	execution/builtins/run_builtins.c \
	execution/builtins/builtin_utils.c \
	execution/signals_parent.c \
	execution/signals_child.c \
	execution/get_exe_path.c \
	execution/configure_redir.c \
	parsing/get_args.c \
	parsing/write_heredoc.c \
	parsing/heredoc_command.c \
	execution/exec_tree.c \
	execution/exec_cmd.c \
	execution/exec_cmd2.c \
	execution/create_pipe_and_fork.c \
	execution/int_tab_realloc.c \
	execution/exec_utils.c \
	main_utils.c \
	main_utils2.c \
	main.c

OBJ_DIR = objects
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

LINKS = $(LIBFT) -lreadline

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINKS) -o $(NAME)
# 	dsymutil $@

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: parsing/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: parsing/variable_expansion/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: execution/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: execution/builtins/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	make clean -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re