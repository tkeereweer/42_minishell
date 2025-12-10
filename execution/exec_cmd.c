/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:44:41 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 10:24:52 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exec_non_builtin(t_data *data, t_node *cmd)
{
	char	*exec_path;
	int		err_flag;

	err_flag = 0;
	exec_path = get_exe_path(data, cmd->left_child->content.tab[0], &err_flag);
	if (exec_path == NULL)
		cmd_not_found(cmd->left_child->content.tab[0], data);
	if (err_flag == 1)
		exec_fail(exec_path, cmd->left_child->content.tab[0], data);
	if (exec_path[0] == '\0')
		exit(0);
	if (execve(exec_path, cmd->left_child->content.tab, data->env) == -1)
		exec_fail(exec_path, cmd->left_child->content.tab[0], data);
}

//this function is in a child process
static int	exec_child(t_node *cmd, t_data *data, int mode)
{
	int		out;
	int		in;
	int		res;

	out = 0;
	in = 0;
	if (cmd->right_child)
	{
		res = configure_redir(cmd->right_child, data, &in, &out);
		if (res == -2)
			return (redir_error(cmd->right_child->content.redir.path, 0));
		if (res == -3)
			return (redir_error("heredoc", 0));
		if (res == -1 && errno == EACCES)
			permission_error_fd(cmd, mode);
		if (res < 0)
			exit(res);
	}
	if (mode != 4)
		config_pipes_modes_123(data, in, out, mode);
	if (!is_builtin(cmd->left_child->content.tab[0]))
		exec_non_builtin(data, cmd);
	else
		exec_builtin_in_child(data, cmd);
	exit(0);
}

static int	exec_mode_4(t_data *data, t_node *cmd, int mode)
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

static int	exec_modes_123(t_data *data, t_node *cmd, int mode)
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
