/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:46:32 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 10:26:01 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_signum;

static t_node	*first_cmd(t_node *node)
{
	while (node->left_child != NULL && node->left_child->type != ARGS)
		node = node->left_child;
	return (node);
}

static t_node	*last_cmd(t_node *node)
{
	while (node->right_child != NULL && node->right_child->type != REDIR)
		node = node->right_child;
	return (node);
}

static int	find_cmd_mode(t_node *node, t_node *root)
{
	if (node->parent->left_child == node && node->parent->right_child == NULL)
		return (4);
	else if (node == first_cmd(root))
		return (1);
	else if (node == last_cmd(root))
		return (3);
	else
		return (2);
}

static int	exec_pipeline(t_node *node, t_data *data, t_node *pipeline_root)
{
	int	ret;

	ret = 0;
	if (node == NULL || (node->parent != NULL && node->parent->type == CMD))
		return (0);
	ret = exec_pipeline(node->left_child, data, pipeline_root);
	if (ret != 0)
		return (ret);
	if (node->type == CMD)
	{
		if (expand_vars(&node->left_child->content.tab, data) == -1)
			return (-1);
		ret = exec_cmd(node, data, find_cmd_mode(node, pipeline_root));
		return (ret);
	}
	ret = exec_pipeline(node->right_child, data, pipeline_root);
	return (ret);
}

int	exec_tree(t_node *node, t_data *data)
{
	static int	exit_status;
	int			ret;
	int			mode;

	if (node->type == CMD || (node->parent != NULL
			&& node->parent->type == PIPELINE))
		return (0);
	if (exec_tree(node->left_child, data) == -1)
		return (-1);
	if (node->type == PIPELINE)
	{
		mode = find_cmd_mode(node->left_child, node);
		ret = exec_pipeline(node, data, node);
		exit_status = set_exit_status(node, data, mode, ret);
		if (exit_status == -1)
			return (-1);
	}
	if ((exit_status == 0 && node->type == LOGIC && node->content.logic == AND)
		|| (exit_status != 0 && node->type == LOGIC
			&& node->content.logic == OR))
	{
		if (exec_tree(node->right_child, data) == -1)
			return (-1);
	}
	return (0);
}
