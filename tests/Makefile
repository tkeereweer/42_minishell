# **************************************************************************** #
#                                                                              #
#    Makefile for Minishell Builtin Tests                                     #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
TEST_FLAGS = -DTESTING
INCLUDES = -I.. -I../libft/src

# Directories
LIBFT_DIR = ../libft
LIBFT = $(LIBFT_DIR)/libft.a
BUILTIN_DIR = ../execution/builtins
PARSING_DIR = ../parsing

# Object files for builtins (compile without main)
BUILTIN_OBJS = ft_echo_builtin.o ft_pwd_builtin.o ft_cd_builtin.o \
               ft_env_builtin.o ft_export_builtin.o ft_unset_builtin.o \
               ft_exit_builtin.o

# Helper objects
HELPER_OBJ = tab_realloc.o
EXPANSION_OBJS = env_vars.o wildcards.o

# Test executables
TESTS = test_echo test_pwd test_cd test_env test_export test_unset test_exit test_expand_vars

# Colors for output
GREEN = \033[0;32m
BLUE = \033[0;34m
NC = \033[0m

all: $(LIBFT) $(TESTS)

$(LIBFT):
	@echo "$(BLUE)Building libft...$(NC)"
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

# Compile builtin object files (with TESTING flag to exclude main functions)
ft_echo_builtin.o: $(BUILTIN_DIR)/ft_echo.c
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) -c $< -o $@

ft_pwd_builtin.o: $(BUILTIN_DIR)/ft_pwd.c
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) -c $< -o $@

ft_cd_builtin.o: $(BUILTIN_DIR)/ft_cd.c
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) -c $< -o $@

ft_env_builtin.o: $(BUILTIN_DIR)/ft_env.c
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) -c $< -o $@

ft_export_builtin.o: $(BUILTIN_DIR)/ft_export.c
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) -c $< -o $@

ft_unset_builtin.o: $(BUILTIN_DIR)/ft_unset.c
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) -c $< -o $@

ft_exit_builtin.o: $(BUILTIN_DIR)/ft_exit.c
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) -c $< -o $@

# Compile helper objects
tab_realloc.o: $(PARSING_DIR)/parsing_utils2.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile expansion objects
env_vars.o: $(PARSING_DIR)/variable_expansion/env_vars.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

wildcards.o: $(PARSING_DIR)/variable_expansion/wildcards.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build test_echo
test_echo: test_echo.c ft_echo_builtin.o $(LIBFT)
	@echo "$(BLUE)Compiling test_echo...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ test_echo.c ft_echo_builtin.o $(LIBFT)

# Build test_pwd
test_pwd: test_pwd.c ft_pwd_builtin.o $(LIBFT)
	@echo "$(BLUE)Compiling test_pwd...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ test_pwd.c ft_pwd_builtin.o $(LIBFT)

# Build test_cd
test_cd: test_cd.c ft_cd_builtin.o $(LIBFT)
	@echo "$(BLUE)Compiling test_cd...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ test_cd.c ft_cd_builtin.o $(LIBFT)

# Build test_env
test_env: test_env.c ft_env_builtin.o $(LIBFT)
	@echo "$(BLUE)Compiling test_env...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ test_env.c ft_env_builtin.o $(LIBFT)

# Build test_export (needs tab_realloc helper)
test_export: test_export.c ft_export_builtin.o tab_realloc.o $(LIBFT)
	@echo "$(BLUE)Compiling test_export...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ test_export.c ft_export_builtin.o tab_realloc.o $(LIBFT)

# Build test_unset
test_unset: test_unset.c ft_unset_builtin.o $(LIBFT)
	@echo "$(BLUE)Compiling test_unset...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ test_unset.c ft_unset_builtin.o $(LIBFT)

# Build test_exit
test_exit: test_exit.c ft_exit_builtin.o $(LIBFT)
	@echo "$(BLUE)Compiling test_exit...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ test_exit.c ft_exit_builtin.o $(LIBFT)

# Build test_expand_vars (needs expansion and helper objects)
test_expand_vars: test_expand_vars.c env_vars.o wildcards.o tab_realloc.o $(LIBFT)
	@echo "$(BLUE)Compiling test_expand_vars...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ test_expand_vars.c env_vars.o wildcards.o tab_realloc.o $(LIBFT)

# Run all tests
test: all
	@echo ""
	@echo "$(GREEN)========================================$(NC)"
	@echo "$(GREEN)  Running All Builtin Tests$(NC)"
	@echo "$(GREEN)========================================$(NC)"
	@echo ""
	@./run_all_tests.sh

# Clean test executables and object files
clean:
	@echo "Cleaning test executables and objects..."
	@rm -f $(TESTS) $(BUILTIN_OBJS) $(HELPER_OBJ) $(EXPANSION_OBJS)

# Clean everything including libft
fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory

# Rebuild everything
re: fclean all

.PHONY: all test clean fclean re
