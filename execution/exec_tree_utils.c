/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 11:26:08 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/09 11:29:43 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	wait_for_pids(t_data *data)
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

int	set_exit_status(t_node *node, t_data *data, int mode, int ret)
{
	int	exit_status;

	if (mode == 4
		&& is_builtin(node->left_child->left_child->content.tab[0]))
		exit_status = ret;
	else if (ret != 0)
	{
		exit_status = wait_for_pids(data);
		return (clean_data(data), -1);
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
	return (clean_data(data), exit_status);
}
