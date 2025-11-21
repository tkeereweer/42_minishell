/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:46:51 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/21 16:16:28 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_cmd_children(t_node *cmd, t_list **pipeline)
{
	while (*pipeline != NULL && (*pipeline)->content->type != PIPE)
	{
		if ((*pipeline)->content->type == ARGS)
			cmd->left_child = (*pipeline)->content;
		else if ((*pipeline)->content->type == REDIR)
			cmd->right_child = (*pipeline)->content;
		*pipeline = (*pipeline)->next;
	}
	if (*pipeline != NULL)
		*pipeline = (*pipeline)->next;
}

int	is_last_pipe(t_list *pipeline)
{
	while (pipeline != NULL)
	{
		if (pipeline->content->type == PIPE)
			return (0);
		pipeline = pipeline->next;
	}
	return (1);
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
			{
				node->right_child = node_new(cont, PIPELINE);
				if (node->right_child == NULL)
					return (1);
				node->right_child->parent = node;
				node = node->right_child;
				if (populate_cmd_tree(pipeline, node) == 1)
					return (1);
			}
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

int	create_cmd_trees(t_node *node)
{
	t_list	*pipeline;
	t_list	*start_list;

	if (node == NULL)
		return (0);
	pipeline = NULL;
	create_cmd_trees(node->left_child);
	create_cmd_trees(node->right_child);
	if (node->type == PIPELINE)
	{
		pipeline = pipeline_list(node->content.str);
		if (!pipeline)
			printf("pipeline list error\n");
		start_list = pipeline;
		free(node->content.str);
		if (populate_cmd_tree(&pipeline, node) == 1)
			return (1);
		free_pipeline_list(start_list);
	}
	return (0);
}
