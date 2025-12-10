/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:54:05 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 11:58:43 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// free malloced pointers in tree->content
void	free_tree(t_node *tree)
{
	if (tree == NULL)
		return ;
	free_tree(tree->left_child);
	free_tree(tree->right_child);
	if (tree->type == ARGS)
		free_split(tree->content.tab);
	else if (tree->type == REDIR)
		free(tree->content.redir.path);
	free(tree);
}

t_node	*node_new(t_content content, t_type type)
{
	t_node	*new;

	new = (t_node *) malloc(sizeof(t_node));
	if (new == NULL)
		return (NULL);
	new->type = type;
	if (type == LOGIC)
		new->content.logic = content.logic;
	else if (type == PIPELINE)
		new->content.str = content.str;
	else if (type == CMD)
		new->content.str = content.str;
	else if (type == ARGS)
		new->content.str = content.str;
	else if (type == REDIR)
		new->content.redir = content.redir;
	else
		new->content = content;
	new->left_child = NULL;
	new->right_child = NULL;
	new->parent = NULL;
	return (new);
}

void	free_pipeline_list(t_list *pipeline)
{
	t_list	*next;

	while (pipeline != NULL)
	{
		next = pipeline->next;
		if (pipeline->content->type == PIPE)
			free(pipeline->content);
		free(pipeline);
		pipeline = next;
	}
}

void	handle_error_pipeline_list(t_node *node)
{
	char	*str;

	str = "minishell: syntax error near unexpected token: ";
	if (node->parent == NULL || node->parent->right_child == node)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd("'newline'\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd(str, STDERR_FILENO);
		if (node->parent->content.logic == AND)
			ft_putstr_fd("'&&'\n", STDERR_FILENO);
		else
			ft_putstr_fd("'||'\n", STDERR_FILENO);
	}
}

char	*str_realloc(char *ptr, size_t size)
{
	char	*out;

	out = (char *) malloc(size);
	if (out == NULL)
	{
		if (ptr)
			free(ptr);
		return (NULL);
	}
	if (ptr)
	{
		ft_strlcpy(out, ptr, size);
		free(ptr);
	}
	return (out);
}
