/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:10:32 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 13:14:57 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _DEFAULT_SOURCE

# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <stdio.h>
# include <fcntl.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <linux/limits.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include "libft/src/libft.h"

typedef enum e_type
{
	LOGIC,
	PAR,
	PIPELINE,
	CMD,
	ARGS,
	REDIR,
	PIPE
}	t_type;

typedef enum e_logic
{
	AND,
	OR
}	t_logic;

typedef enum e_redir_type
{
	APPEND,
	READ,
	WRITE,
	HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	kind;
	char			*path;
}	t_redir;

typedef union u_content
{
	t_logic	logic;
	char	parenthesis;
	char	*str;
	char	**tab;
	t_redir	redir;
	void	*empty;
}	t_content;

typedef struct s_node
{
	t_type			type;
	t_content		content;
	struct s_node	*left_child;
	struct s_node	*right_child;
	struct s_node	*parent;
}	t_node;

typedef struct s_data
{
	char	**env;
	char	*default_path;
	char	*line;
	char	*prompt;
	int		child_cnt;
	int		cmd_cnt;
	int		**pipe_tab;
	pid_t	*pid_tab;
	t_node	*tree;
	int		exit_status;
}	t_data;

//filer struct for get_args
typedef struct s_make_tab
{
	int		word_count;
	int		i;
	int		j;
	char	*str;
	char	**tab;
}	t_make_tab;

t_node		*node_new(t_content content, t_type type);
t_node		*create_logic_tree(t_list *list);
int			create_cmd_trees(t_node *node);
void		free_tree(t_node *tree);
//parsing
t_list		*clean_node_list(char **line, char ***path_tab, t_data *data);
int			open_par_token(t_list **list);
int			close_par_token(t_list **list);
int			pipeline_token(char *str, t_list **list);
int			and_token(t_list **list);
int			or_token(t_list **list);
int			build_node_list(char **line, t_list **list, char ***path_tab);
char		is_sep(char *str);
char		is_logic(char *str);
char		is_redir(char *str);
void		del_linked(t_node *lst);
t_list		*list_error(t_list **list, char *str, t_list **temp);
int			pipe_token(t_list **lst);
int			write_token(t_list **lst, char *filepath);
int			read_token(t_list **lst, char *filepath);
int			append_token(t_list **lst, char *filepath);
int			heredoc_token(t_list **lst, char *limiter);
int			arg_token(char *word, t_list **lst);
int			tokenizer_error(char *str);
int			empty_end(char *line, int *j, int *i);
char		*remove_redir(char *str, int start, int end);
int			pipeline_list(char *line, t_list **head);
int			pipeline_list_error(char *str, t_list **temp, int result);
int			tokenize_word(char *line, int *i, char **str, int space);
int			iterate_over_quotes(char *line, int *j);
int			redir_token(t_list **lst, char *line, int *i);
int			return_1_subpipe(char ***subpipe, char *line);
int			build_subpipe(char ***subpipe, char *line, int *i);
int			clean_args_nodes(t_list **head);
int			increment_subpipe(char ***subpipe, char *line, int *i, int *j);
int			check_quote_balance(char **line, int *j, char ***tab);
int			sep_tokenizer(char *line, int *i, t_list **list);
t_list		*set_temp(t_list **list, t_list *temp);
int			separator_logic(char *line, int *i, t_list *temp, t_list **list);
t_list		*syntax_error(t_list **lst);
int			check_unclosed_par(t_list **list);
int			tab_len(char **tab);
char		**args_tab(char *str);
char		*replace_with_path(char *dest, char *path, int start, int end);
void		free_pipeline_list(t_list *pipeline);
void		handle_error_pipeline_list(t_node *node);
int			quoted_heredoc_error(int quoted_heredoc);
int			is_last_pipe(t_list *pipeline);
t_make_tab	init_struct(char *str);
// variable expansion
int			expand_envvars(char **str, t_data *data);
int			expand_envvars_redir(char **str, t_data *data);
int			expand_envvar_str(char **str, int i, t_data *data, int mode);
int			expand_wildcards(char ***tab, int i, char *pat);
int			expand_wildcards_redir(char **path, char *pat);
int			expand_vars(char ***tab, t_data *data);
int			expand_vars_redir(char **path, t_data *data);
int			remove_quotes(char **str);
char		*ft_getenv(char *var, char **env);
int			has_envvar(char *str, int i, int mode);
int			envvar_len(char *str);
int			expand_env_in_big_quotes(char **str, int *i, t_data *data);
int			in_quotes(char *str, int idx);
void		if_small_quote(int *i, char **str);
int			match_pat(char *str, char *pat);
//builtins
int			ft_cd(char *path, t_data *data);
int			ft_pwd(void);
void		ft_echo(char **args);
void		ft_exit(char **argv, t_data *data, int fd1, int fd2);
int			ft_export(char **argv, t_data *data);
int			ft_export_var(char *key_val, t_data *data);
void		ft_unset(char **tab, t_data *data);
void		ft_env(t_data *data);
int			run_builtins(char **argv, t_data *data, int fd1, int fd2);
char		**get_envvar_pointer(char *var, char **env);
int			get_key_len(char *key_val);
int			valid_key(char *key, int key_len);
int			add_key(char *key_val, t_data *data);
int			key_already_exists(char **curr_key_val, char *key_val);
int			has_wc(char *str);
//utils
char		*ft_strcat(char *dst, char *src);
char		*ft_getenv(char *var, char **env);
int			handle_signals_parent(int mode);
int			handle_signals_child(void);
char		**get_envvar_pointer(char *var, char **env);
char		*my_realloc(char *ptr, size_t size);
int			ft_strcmp(const char *s1, const char *s2);
char		*str_realloc(char *ptr, size_t size);
void		free_split(char **tab);
int			subpipe_error(int code, char **subpipe);
char		**tab_realloc(char **tab, int n);
char		*remove_redir(char *str, int start, int end);
//heredoc
char		**heredoc(char **path_tab, char *limiter,
				int quoted_heredoc, int *err_flag);
int			set_heredoc(char **line, int *j, char ***tab);
void		clean_path_tab(char **path_tab);
int			write_heredoc(char *limiter, int fd);
char		**temp_filepath(char **tmp_name, int count, int quoted_heredoc);
char		**try_filepath(char **filepath, int count);
// execution
int			exec_tree(t_node *node, t_data *data);
int			set_exit_status(t_node *node, t_data *data, int mode, int ret);
int			exec_cmd(t_node *cmd, t_data *data, int mode);
int			is_builtin(char *name);
void		clean_exit(t_data *data, char *line, char *prompt);
void		clean_data(t_data *data);
int			populate_cmd_tree(t_list **pipeline, t_node *node);
char		*get_exe_path(t_data *data, char *cmd, int *err_flag);
void		cmd_not_found(char *cmd, t_data *data);
void		permission_error(char *path, t_data *data);
int			permission_error_fd(t_node *cmd, int mode);
void		exec_fail(char *path, char *cmd, t_data *data);
int			configure_redir(t_node *redir, t_data *data, int *in_redir,
				int *out_redir);
int			create_pipe(t_data *data, int mode);
int			create_pid(t_node *arg, t_data *data);
int			**int_tab_realloc(int **tab, int size);
int			valid_char(char *str);
int			redir_error(char *path, int mode);
int			ambig_redirect(void);
void		dup_old_streams(int old_stdin, int old_stdout);
char		*init_path(int *i, char **ret, char **temp1, char *cmd);
int			is_directory(char *path);
int			has_a_slash(char *cmd);
void		config_pipes_modes_123(t_data *data, int in, int out, int mode);
int			exec_builtin(t_node *cmd, t_data *data, int mode);
void		exec_builtin_in_child(t_data *data, t_node *cmd);
int			setup_oldstds(int *old_stdin, int *old_stdout);
void		set_errno_isdir(int *err_flag);
//main
int			only_whitespace(char *line);
void		clean_exit(t_data *data, char *line, char *prompt);
char		*empty_env_prompt(void);
int			set_minimal_env(t_data *data);
void		handle_next_cmd(t_data *data, char **envp);
char		*build_prompt(t_data *data, char **envp);
t_data		init_data(char **envp);
int			run_line(char **line, t_data *data, char **temp);
int			copy_env(t_data *data, char **envp);
#endif
