SRCS = parsing/logic_tree.c \
	parsing/logic_tree_utils.c \
	parsing/command_tree.c \
	parsing/build_node_list.c \
	parsing/parse_pipeline.c \
	parsing/parsing_error.c \
	parsing/parsing_utils2.c \
	parsing/build_subpipe.c \
	parsing/parsing_utils4.c \
	parsing/parsing_utils5.c \
	parsing/clean_node_list.c \
	parsing/parsing_utils7.c \
	parsing/token_redir.c \
	parsing/token.c \
	parsing/token2.c \
	parsing/variable_expansion/env_vars.c \
	parsing/variable_expansion/env_vars_redir.c \
	parsing/variable_expansion/env_vars_utils.c \
	parsing/variable_expansion/wildcards.c \
	parsing/variable_expansion/wildcards_redir.c \
	parsing/variable_expansion/wildcards_utils.c \
	parsing/variable_expansion/expand_vars.c \
	parsing/variable_expansion/match_pattern.c \
	execution/builtins/ft_export.c \
	execution/builtins/ft_unset.c \
	execution/builtins/ft_env.c \
	execution/builtins/ft_cd.c \
	execution/builtins/ft_pwd.c \
	execution/builtins/ft_exit.c \
	execution/builtins/ft_echo.c \
	execution/builtins/run_builtins.c \
	execution/builtins/ft_export_utils.c \
	execution/signals_parent.c \
	execution/signals_child.c \
	execution/get_exe_path.c \
	execution/get_exe_path_utils.c \
	execution/configure_redir.c \
	parsing/get_args.c \
	parsing/write_heredoc.c \
	parsing/heredoc_command.c \
	execution/exec_tree.c \
	execution/exec_cmd.c \
	execution/exec_cmd_builtin.c \
	execution/create_pipe_and_fork.c \
	execution/int_tab_realloc.c \
	execution/exec_cmd_utils.c \
	execution/exec_tree_utils.c \
	execution/exec_errors.c \
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