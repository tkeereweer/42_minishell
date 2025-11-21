/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:10:32 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/21 12:04:30 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft/src/libft.h"

typedef enum e_type
{
	LOGIC, PAR, PIPELINE, CMD, ARGS, REDIR, PIPE
}	t_type;

typedef enum e_logic
{
	AND, OR
}	t_logic;

typedef enum e_redir_type
{
	APPEND, READ, WRITE, HEREDOC
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
	char    **tab;
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

t_node	*node_new(t_content content, t_type type);
t_node	*create_logic_tree(t_list *list);
int		create_cmd_trees(t_node *node);
void	free_tree(t_node *tree);
//parsing
t_list	*clean_node_list(char *line);
int 	open_par_token(t_list **list);
int 	close_par_token(t_list **list);
int		pipeline_token(char *str, t_list **list);
int		and_token(t_list **list);
int		or_token(t_list **list);
int 	build_node_list(char *line, t_list **list);
char	is_sep(char *str);
char	is_logic(char *str);
char	is_redir(char *str);
void 	del_linked(t_node *lst);
t_list	*list_error(t_list **list,char *str, t_list **temp);
int		pipe_token(t_list **lst);
int		write_token(t_list **lst, char *filepath);
int		read_token(t_list **lst, char *filepath);
int		append_token(t_list **lst, char *filepath);
int		heredoc_token(t_list **lst, char *limiter);
int		word_token(char *word, t_list **lst);
int		cmd_token(char *word, t_list **lst);
int		arg_token(char *word, t_list **lst);
int		tokenizer_error(char *str);
int		empty_end(char *line, int *j, int *i);
char	*remove_redir(char *str, int start, int end);
t_list	*pipeline_list(char *line);
int	tokenize_word(char *line, int *i, char **str, int space);

//testing
void	draw_tree(t_node *root);
void	free_split(char **tab);
int		subpipe_error(int code, char **subpipe);
char	**tab_realloc(char **tab, int n);
char	*remove_redir(char *str, int start, int end);
int		tab_len(char **tab);
char	**args_tab(char *str);
//heredoc
char	**heredoc(char **path_tab, char *limiter);
#endif

