/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:46:51 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 11:58:39 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	add_cmd_children(t_node *cmd, t_list **pipeline)
{
	t_node	*tmp;

	while (*pipeline != NULL && (*pipeline)->content->type != PIPE)
	{
		if ((*pipeline)->content->type == ARGS)
		{
			cmd->left_child = (*pipeline)->content;
			cmd->left_child->parent = cmd;
		}
		else if ((*pipeline)->content->type == REDIR)
		{
			tmp = cmd;
			while (tmp->right_child != NULL)
				tmp = tmp->right_child;
			tmp->right_child = (*pipeline)->content;
			tmp->right_child->parent = tmp;
		}
		*pipeline = (*pipeline)->next;
	}
	if (*pipeline != NULL)
		*pipeline = (*pipeline)->next;
}

static int	new_pipeline_node(t_node *node, t_content cont, t_list **pipeline)
{
	node->right_child = node_new(cont, PIPELINE);
	if (node->right_child == NULL)
		return (1);
	node->right_child->parent = node;
	node = node->right_child;
	if (populate_cmd_tree(pipeline, node) == 1)
		return (1);
	return (0);
}

int	populate_cmd_tree(t_list **pipeline, t_node *node)
{
	t_content	cont;

	cont.empty = NULL;
	while (*pipeline != NULL)
	{
		node->left_child = node_new(cont, CMD);
		if (node->left_child == NULL)
			return (1);
		node->left_child->parent = node;
		add_cmd_children(node->left_child, pipeline);
		if (*pipeline != NULL)
		{
			if (is_last_pipe(*pipeline) == 0)
				return (new_pipeline_node(node, cont, pipeline));
			else
			{
				node->right_child = node_new(cont, CMD);
				if (node->right_child == NULL)
					return (1);
				node->right_child->parent = node;
				add_cmd_children(node->right_child, pipeline);
			}
		}
	}
	return (0);
}

static int	pipeline_error(int res, t_node *node, t_list *pipeline)
{
	if (res == -1)
		handle_error_pipeline_list(node);
	free_pipeline_list(pipeline);
	if (res == 0)
		return (1);
	else
		return (2);
}

int	create_cmd_trees(t_node *node)
{
	t_list	*pipeline;
	t_list	*start_list;
	int		res;

	if (node == NULL)
		return (0);
	pipeline = NULL;
	res = create_cmd_trees(node->left_child);
	if (res > 0)
		return (res);
	res = create_cmd_trees(node->right_child);
	if (res > 0)
		return (res);
	if (node->type == PIPELINE)
	{
		res = pipeline_list(node->content.str, &pipeline);
		if (res <= 0)
			return (pipeline_error(res, node, pipeline));
		start_list = pipeline;
		free(node->content.str);
		if (populate_cmd_tree(&pipeline, node) == 1)
			return (free_pipeline_list(start_list), 1);
		free_pipeline_list(start_list);
	}
	return (0);
}
