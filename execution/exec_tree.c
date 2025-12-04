/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:46:32 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/04 11:17:17 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_signum;

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
		return (4);
	else if (node == first_cmd(root))
		return (1);
	else if (node == last_cmd(root))
		return (3);
	else
		return (2);
}

int	exec_pipeline(t_node *node, t_data *data, t_node *pipeline_root)
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
		// if (ret != 0)
		// 	return (-1);
		return (ret);
	}
	ret = exec_pipeline(node->right_child, data, pipeline_root);
	// if (ret != 0)
	// 	return (ret);
	return (ret);
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
	if (data->pipe_tab)
	{
		while (i < data->cmd_cnt)
		{
			free(data->pipe_tab[i]);
			i++;
		}
		free(data->pipe_tab);
		data->pipe_tab = NULL;
	}
	free(data->pid_tab);
	data->pid_tab = NULL;
	data->child_cnt = 0;
	data->cmd_cnt = 0;
}

int	exec_tree(t_node *node, t_data *data)
{
	static int	exit_status;
	int			ret;
	int			mode;

	if (node->type == CMD || (node->parent != NULL && node->parent->type == PIPELINE))
		return (0);
	if (exec_tree(node->left_child, data) == -1)
		return (-1);
	if (node->type == PIPELINE)
	{
		mode = find_cmd_mode(node->left_child, node);
		ret = exec_pipeline(node, data, node);
		if (mode == 4 && is_builtin(node->left_child->left_child->content.tab[0]))
			exit_status = ret;
		else if (ret != 0)
		{
			if (ret != -5)
				exit_status = wait_for_pids(data);
			clean_data(data);
			return (-1);
		}
		else
		{
			exit_status = wait_for_pids(data);
			if (WIFEXITED(exit_status))
				exit_status = WEXITSTATUS(exit_status);
			else if (WIFSIGNALED(exit_status))
			{
				if (exit_status < 131)
					exit_status = 130;
				ft_printf("\n");
			}
		}
		data->exit_status = exit_status;
		clean_data(data);
	}
	if ((exit_status == 0 && node->type == LOGIC && node->content.logic == AND) || (exit_status != 0 && node->type == LOGIC && node->content.logic == OR))
	{
		if (exec_tree(node->right_child, data) == -1)
			return (-1);
	}
	return (0);
}
