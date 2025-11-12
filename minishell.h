/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:10:32 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/12 15:47:22 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft/src/libft.h"

typedef enum e_type
{
	LOGIC, PAR, PIPELINE, CMD, ARGS, REDIR, END
}	t_type;

typedef enum e_logic
{
	AND, OR
}	t_logic;

typedef enum e_redir_type
{
	PIPE, APPEND, READ, WRITE
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
	t_redir	redir;
}	t_content;

typedef struct s_node
{
	t_type			type;
	t_content		content;
	struct s_node	*left_child;
	struct s_node	*right_child;
	struct s_node	*parent;
}	t_node;

//parsing
t_node	*node_new(t_content content, t_type type);
int open_par_token(t_list **list);
int close_par_token(t_list **list);
int	pipeline_token(char *str, t_list **list);
int	and_token(t_list **list);
int	or_token(t_list **list);

#endif

