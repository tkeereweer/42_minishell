/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:44:41 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/26 11:23:01 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	dup_fds(int fd, t_redir_type kind, int *in_redir, int *out_redir)
{
	if (fd == -1)
		return (-1);//exit code 1
	if (kind == WRITE || kind == APPEND)
	{
		dup2(STDOUT_FILENO, fd);
		*out_redir = 1;
	}
	if (kind == READ || kind == HEREDOC)
	{
		dup2(STDIN_FILENO, fd);
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
	while (redir)
	{
		path = temp->content.redir.path;
		kind = temp->content.redir.kind;
		if (kind == WRITE)
			fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (kind == APPEND)
			fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (kind == READ)
			fd = open(path, O_RDONLY);
		else if (kind == HEREDOC)
			fd = open(path, O_RDONLY);
		if (dup_fds(fd, kind, in_redir, out_redir))
			return (-1);
		temp = redir->right_child;
	}
	return (1);
}

//need read end of previous for stdin and write end of current for stdout
int exec_builtin(t_node *cmd, t_data *data, int mode)
{
	int	out;
	int in;

	out = 0;
	in = 0;
	if (cmd->right_child)
		if (configure_redir(cmd->right_child, data, &in, &out) == -1)
			return (-1);
	//config_pipe_parent()
	if (mode > 1 && !in)
	{
		dup2(data->pipe_tab[data->cmd_cnt - 2][0], STDIN_FILENO);
		close(data->pipe_tab[data->cmd_cnt - 2][0]);
	}
	if (mode < 3 && !out)
	{
		dup2(data->pipe_tab[data->cmd_cnt - 1][1], STDOUT_FILENO);
		close(data->pipe_tab[data->cmd_cnt - 1][1]);
	}
	//match builtin
	//exec_builtin()
	//error handling and return
}

int exec_child(t_node *cmd, t_data *data, int mode)
{	
	int	out;//bools to know wether to setup in or out
	int in;

	out = 0;
	in = 0;
	if (cmd->right_child)
		if (configure_redir(cmd->right_child, data, &out, &in) == -1)
			return (-1);
	//config_pipe_child()
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
	//find_path
	//execve()
	//error_catch and exit
}

int	exec_cmd(t_node *cmd, int mode, t_data *data)
{
	data->cmd_cnt++;
	create_pipe(data, mode);
	create_fork(cmd->left_child, data, mode);
	if (is_builtin(cmd->left_child->content.tab[0]))
	{
		data->pid_tab[data->cmd_cnt - 1] = -1;
		return (exec_builtin(cmd, data, mode));
	}
	else
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
	}
	return (0);
}
