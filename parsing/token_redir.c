/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:38:45 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/14 16:58:14 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pipe_token(t_list **lst)
{
	t_content	temp_cont;
	t_type		temp_type;
	t_list		*temp;
	t_node		*temp_node;

	temp_cont.str = NULL;
	temp_type = PIPELINE;
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (-1);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (-1);
	}
	ft_lstadd_back(lst, temp);
	return (1);
}

//free filepath after exec
int	write_token(t_list **lst, char *filepath)
{
	t_content	temp_cont;
	t_type		temp_type;
	t_list		*temp;
	t_node		*temp_node;

	temp_cont.redir.kind = WRITE;
    temp_cont.redir.path = filepath;
	temp_type = REDIR;
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (-1);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (-1);
	}
	ft_lstadd_back(lst, temp);
	return (1);
}

int	read_token(t_list **lst, char *filepath)
{
	t_content	temp_cont;
	t_type		temp_type;
	t_list		*temp;
	t_node		*temp_node;

	temp_cont.redir.kind = READ;
    temp_cont.redir.path = filepath;
	temp_type = REDIR;
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (-1);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (-1);
	}
	ft_lstadd_back(lst, temp);
	return (1);
}

int	append_token(t_list **lst, char *filepath)
{
	t_content	temp_cont;
	t_type		temp_type;
	t_list		*temp;
	t_node		*temp_node;

	temp_cont.redir.kind = APPEND;
    temp_cont.redir.path = filepath;
	temp_type = REDIR;
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (-1);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (-1);
	}
	ft_lstadd_back(lst, temp);
	return (1);
}
//free limiter after exec
int	heredoc_token(t_list **lst, char *limiter)
{
	t_content	temp_cont;
	t_type		temp_type;
	t_list		*temp;
	t_node		*temp_node;

	temp_cont.redir.kind = HEREDOC;
    temp_cont.redir.path = limiter;
	temp_type = REDIR;
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (-1);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (-1);
	}
	ft_lstadd_back(lst, temp);
	return (1);
}
