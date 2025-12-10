/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:21:16 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 10:16:06 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	permission_error_fd(t_node *cmd, int mode)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->right_child->content.redir.path, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	if (mode == 4 && is_builtin(cmd->left_child->content.tab[0]))
		return (1);
	exit(1);
}

int	setup_oldstds(int *old_stdin, int *old_stdout)
{
	*old_stdin = -1;
	*old_stdout = -1;
	*old_stdin = dup(STDIN_FILENO);
	*old_stdout = dup(STDOUT_FILENO);
	if (*old_stdin == -1 || *old_stdout == -1)
		return (-1);
	return (0);
}

void	dup_old_streams(int old_stdin, int old_stdout)
{
	dup2(old_stdin, STDIN_FILENO);
	close(old_stdin);
	dup2(old_stdout, STDOUT_FILENO);
	close(old_stdout);
}

void	config_pipes_modes_123(t_data *data, int in, int out, int mode)
{
	if (mode > 1)
	{
		if (mode == 2)
			close(data->pipe_tab[data->cmd_cnt - 1][0]);
		if (!in)
			dup2(data->pipe_tab[data->cmd_cnt - 2][0], STDIN_FILENO);
		close(data->pipe_tab[data->cmd_cnt - 2][0]);
	}
	if (mode < 3)
	{
		if (!out)
			dup2(data->pipe_tab[data->cmd_cnt - 1][1], STDOUT_FILENO);
		close(data->pipe_tab[data->cmd_cnt - 1][1]);
		close(data->pipe_tab[data->cmd_cnt - 1][0]);
	}
}
