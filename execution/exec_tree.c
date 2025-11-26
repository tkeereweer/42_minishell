/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:46:32 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/26 11:27:19 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_node	*first_cmd(t_node *node)
{
	while (node->left_child != NULL && node->left_child->type != ARGS)
		node = node->left_child;
	return (node);
}

t_node	*last_cmd(t_node *node)
{
	while (node->right_child != NULL && node->right_child->type != REDIR)
		node = node->right_child;
	return (node);
}

int	find_cmd_mode(t_node *node, t_node *root)
{
	if (node->parent->left_child == node && node->parent->right_child == NULL)
		return (3);
	else if (node == first_cmd(root))
		return (1);
	else if (node == last_cmd(root))
		return (3);
	else
		return (2);
}

int	exec_cmd(t_node	*node, t_data *data, int mode)
{
	int	i;

	i = 0;
	ft_printf("Mode is %d\n", mode);
	(void) data;
	while (node->left_child->content.tab[i] != NULL)
	{
		ft_printf("%s;\n", node->left_child->content.tab[i]);
		i++;
	}
	return (0);
}

int	exec_pipeline(t_node *node, t_data *data, t_node *pipeline_root)
{
	if (node == NULL || (node->parent != NULL && node->parent->type == CMD))
		return (0);
	if (exec_pipeline(node->left_child, data, pipeline_root) == -1)
		return (-1);
	if (node->type == CMD)
	{
		if(expand_vars(&node->left_child->content.tab, data) == 1)
			return (-1);
		if (exec_cmd(node, data, find_cmd_mode(node, pipeline_root)) == -1)
			return (-1);
	}
	if (exec_pipeline(node->right_child, data, pipeline_root) == -1)
		return (-1);
	return (0);
}

int	wait_for_pids(t_data *data)
{
	int	i;
	int	exit_status;

	i = 0;
	while (i < data->cmd_cnt)
	{
		if (data->pid_tab[i] != -1)
			waitpid(data->pid_tab[i], &exit_status, 0);
		i++;
	}
	return (exit_status);
}

void	clean_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_cnt)
	{
		free(data->pipe_tab[i]);
		i++;
	}
	free(data->pipe_tab);
	data->pipe_tab = NULL;
	free(data->pid_tab);
	data->pid_tab = NULL;
	data->child_cnt = 0;
	data->cmd_cnt = 0;
}

int	exec_tree(t_node *node, t_data *data)
{
	static int	exit_status;

	if (node->type == CMD || (node->parent != NULL && node->parent->type == PIPELINE))
		return (0);
	if (exec_tree(node->left_child, data) == -1)
		return (-1);
	if (node->type == PIPELINE)
	{
		if (exec_pipeline(node, data, node) == -1)
		{
			exit_status = wait_for_pids(data);
			clean_data(data);
			return (-1);
		}
		exit_status = WEXITSTATUS(wait_for_pids(data));
		exit_status = 128;
		clean_data(data);
	}
	if ((exit_status == 128 && node->type == LOGIC && node->content.logic == AND) || (exit_status != 128 && node->type == LOGIC && node->content.logic == OR))
	{
		if (exec_tree(node->right_child, data) == -1)
			return (-1);
	}
	return (0);
}
