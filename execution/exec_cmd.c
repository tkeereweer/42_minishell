/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:44:41 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 10:34:23 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//need read end of previous for stdin and write end of current for stdout
static int	exec_builtin(t_node *cmd, t_data *data, int mode)
{
	int	out;
	int	in;
	int	old_stdin;
	int	old_stdout;
	int	ret;

	out = 0;
	in = 0;
	old_stdin = -1;
	old_stdout = -1;
	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);
	if (old_stdin == -1 || old_stdout == -1)
		return (-1);
	if (cmd->right_child)
	{
		ret = configure_redir(cmd->right_child, data, &in, &out);
		if (ret == -2)
			return (redir_error(cmd->right_child->content.redir.path, mode));
		if (ret == -1)
			return (permission_error_fd(cmd->right_child->content.redir.path, mode));
		if (ret < 0)
			return (1);
	}
	ret = run_builtins(cmd->left_child->content.tab, data, old_stdin, old_stdout);
	dup_old_streams(old_stdin, old_stdout);
	return (ret);
}

static int  exec_mode_4(t_data *data, t_node *cmd, int mode)
{
	data->pid_tab[data->cmd_cnt - 1] = fork();
	if (data->pid_tab[data->cmd_cnt - 1] == -1)
		return (-4);
	if (data->pid_tab[data->cmd_cnt - 1] == 0)
	{
		handle_signals_child();
		exec_child(cmd, data, mode);
	}
	return (0);
}

static int  exec_modes_123(t_data *data, t_node *cmd, int mode)
{
	data->child_cnt++;
	data->pid_tab[data->cmd_cnt - 1] = fork();
	if (data->pid_tab[data->cmd_cnt - 1] == -1)
		return (-4);
	if (data->pid_tab[data->cmd_cnt - 1] == 0)
	{
		handle_signals_child();
		exec_child(cmd, data, mode);
	}
	if (mode < 3)
		close(data->pipe_tab[data->cmd_cnt - 1][1]);
	if (mode > 1)
		close(data->pipe_tab[data->cmd_cnt - 2][0]);
	return (0);
}

int	exec_cmd(t_node *cmd, t_data *data, int mode)
{
	int	ret;

	data->cmd_cnt++;
	ret = create_pipe(data, mode);
	if (ret < 0)
		return (ret);
	ret = create_pid(cmd->left_child, data);
	if (ret < 0)
		return (data->cmd_cnt--, ret);
	if (is_builtin(cmd->left_child->content.tab[0]) && mode == 4)
	{
		data->pid_tab[data->cmd_cnt - 1] = -1;
		return (exec_builtin(cmd, data, mode));
	}
	if (mode != 4)
		return (exec_modes_123(data, cmd, mode));
	else
		return (exec_mode_4(data, cmd, mode));
}
