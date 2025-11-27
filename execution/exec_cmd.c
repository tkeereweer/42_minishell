/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:44:41 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/27 10:24:13 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	dup_fds(int fd, t_redir_type kind, int *in_redir, int *out_redir)
{
	if (fd == -1)
		return (-1);//exit code 1
	if (kind == WRITE || kind == APPEND)
	{
		dup2(fd, STDOUT_FILENO);
		*out_redir = 1;
	}
	if (kind == READ || kind == HEREDOC)
	{
		dup2(fd, STDIN_FILENO);
		*in_redir = 1;
	}
	close (fd);
	return (1);
}

int	configure_redir(t_node *redir, t_data *data, int *in_redir, int *out_redir)
{
	int				fd;
	char			*path;
	t_redir_type	kind;
	t_node			*temp;

	//expand 
	fd = -1;
	temp = redir;
	while (temp)
	{
		path = temp->content.redir.path;
		if (expand_vars_redir(&path, data) == -1)
			return (-1);
		kind = temp->content.redir.kind;
		if (kind == WRITE)
			fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (kind == APPEND)
			fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (kind == READ)
			fd = open(path, O_RDONLY);
		else if (kind == HEREDOC)
			fd = open(path, O_RDONLY);
		if (dup_fds(fd, kind, in_redir, out_redir) == -1)
			return (-1);
		temp = redir->right_child;
	}
	return (1);
}

//need read end of previous for stdin and write end of current for stdout
static int exec_builtin(t_node *cmd, t_data *data, int mode)
{
	int	out;
	int in;
	int old_stdin;
	int old_stdout;
	int ret;

	out = 0;
	in = 0;
	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);
	if (old_stdin == -1 || old_stdout == -1)
		return (-1);
	if (cmd->right_child)
		if (configure_redir(cmd->right_child, data, &in, &out) == -1)
			return (-1);
	ret = run_builtins(cmd->left_child->content.tab, data, mode);
	dup2(old_stdin, STDIN_FILENO);
	close(old_stdin);
	dup2(old_stdout, STDOUT_FILENO);
	close(old_stdout);
	return (ret);
}

static int exec_child(t_node *cmd, t_data *data, int mode)
{	
	int     out;//bools to know wether to setup in or out
	int     in;
	char    *exec_path;

	out = 0;
	in = 0;
	if (cmd->right_child)
		if (configure_redir(cmd->right_child, data, &in, &out) == -1)
			return (-1);
	//config_pipe_child()
	if (mode != 4)
	{
		if (mode > 1)
		{
			close(data->pipe_tab[data->cmd_cnt - 1][0]);
			if (!in)
			{
				dup2(data->pipe_tab[data->cmd_cnt - 2][0], STDIN_FILENO);
				close(data->pipe_tab[data->cmd_cnt - 2][0]);
			}
		}
		if (mode < 3 && !out)
		{
			dup2(data->pipe_tab[data->cmd_cnt - 1][1], STDOUT_FILENO);
			close(data->pipe_tab[data->cmd_cnt - 1][1]);
		}
	}
	if (!is_builtin(cmd->left_child->content.tab[0]))
	{
		exec_path = get_exe_path(data->env, cmd->left_child->content.tab[0]);
		if (exec_path == NULL)
			cmd_not_found(cmd->left_child->content.tab[0]);
		if (execve(exec_path, cmd->left_child->content.tab, data->env) == -1)
			exec_fail(exec_path, cmd->left_child->content.tab[0]);
	}
	else 
		run_builtins(cmd->left_child->content.tab, data, mode);
	return (1);
}

int	exec_cmd(t_node *cmd, t_data *data, int mode)
{
	data->cmd_cnt++;
	create_pipe(data, mode);
	create_fork(cmd->left_child, data);
	if (is_builtin(cmd->left_child->content.tab[0]) && mode == 4)
	{
		data->pid_tab[data->cmd_cnt - 1] = -1;
		return (exec_builtin(cmd, data, mode));
	}
	if (mode !=4)
	{
		data->child_cnt++;
		data->pid_tab[data->cmd_cnt - 1] = fork();
		if (data->pid_tab[data->cmd_cnt - 1] == -1)
			return (-4); //fork error
		if (data->pid_tab[data->cmd_cnt - 1] == 0)
		{
			handle_signals_child();
			return (exec_child(cmd, data, mode));
		}
		if (mode < 3)//in parent
			close(data->pipe_tab[data->cmd_cnt - 1][1]);
		if (mode > 1)
				close(data->pipe_tab[data->cmd_cnt - 2][0]);
		return (0);
	}
	else
	{
		data->pid_tab[data->cmd_cnt - 1] = fork();
		if (data->pid_tab[data->cmd_cnt - 1] == -1)
			return (-4); //fork error
		if (data->pid_tab[data->cmd_cnt - 1] == 0)
		{
			handle_signals_child();
			return (exec_child(cmd, data, mode));
		}
		return (0);
	}
//RE-REDIRECT STDIN  AND OUT IF CALLED IN MAIN PROCESS AFTER BUILTIN OTHEWISE EVERYTHING WRITTEN IN NON CLOSED FD
}
