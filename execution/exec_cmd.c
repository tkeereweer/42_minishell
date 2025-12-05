/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:44:41 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/05 11:22:42 by mturgeon         ###   ########.fr       */
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

static int	redir_error(char *path)
{
	write(STDERR_FILENO, "minishell: ", 11);
	perror(path);
	exit(1);
}

static int	ambig_redirect(void)
{
	write(STDERR_FILENO, "minishell: ", ft_strlen("minishell: "));
	write(STDERR_FILENO, "ambiguous redirect\n", ft_strlen(" ambiguous redirect\n"));
	return (-2);
}

//check for env variable expansion in heredoc
//flag for quoted heredocs is a 'Q' at end of filepath
static int	expand_heredoc(int fd, char *path, t_data *data)
{
	char	*temp;
	char	*file;

	file = NULL;
	if (path[ft_strlen(path) - 1] == 'Q')
		return (1);
	temp = get_next_line(fd);
	if (!temp)
		return (-2);//read/write error
	while (temp)
	{
		file = my_realloc(file, ft_strlen_gnl(file) + ft_strlen(temp) + 1);
		if (!file)
			return (free(temp), -1);//mallocfail
		ft_strncat(file, temp, ft_strlen(temp));
		free(temp);
		temp = get_next_line(fd);
	}
	if (expand_envvar_str(&file, 0, data, 1) == -1) //change to expand_envar_heredoc where it expands regardless of quote type
		return (free(temp), free(file), -1);
	close(fd);
	fd = open(path, O_WRONLY | O_TRUNC);
	if (write(fd, file, ft_strlen(file)) == -1)
		return (free(temp), free(file), -2);
	close(fd);
	open(path, O_RDONLY);//do we have functions for this like lseek ? or flags for open ?
	return (free(temp), free(file), 1);
}

int	configure_redir(t_node *redir, t_data *data, int *in_redir, int *out_redir)
{
	int				fd;
	int				res;
	t_redir_type	kind;

	fd = -1;
	while (redir != NULL)
	{
		res = expand_vars_redir(&(redir->content.redir.path), data);
		if (res == -1)
			return (-1);
		if (res == -2)
			return (ambig_redirect());
		kind = redir->content.redir.kind;
		if (kind == WRITE)
			fd = open(redir->content.redir.path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (kind == APPEND)
			fd = open(redir->content.redir.path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (kind == READ)
			fd = open(redir->content.redir.path, O_RDONLY);
		else if (kind == HEREDOC)
		{
			fd = open(redir->content.redir.path, O_RDONLY);
			res = expand_heredoc(fd, redir->content.redir.path, data);
			if (res < 0)
				return (res);
		}
		if (dup_fds(fd, kind, in_redir, out_redir) == -1)
			return (redir_error(redir->content.redir.path));
		redir = redir->right_child;
	}
	return (1);
}

//need read end of previous for stdin and write end of current for stdout
static int	exec_builtin(t_node *cmd, t_data *data)
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
		if (ret < 0)
			return (ret);
	}
	ret = run_builtins(cmd->left_child->content.tab, data, old_stdin, old_stdout);
	dup2(old_stdin, STDIN_FILENO);
	close(old_stdin);
	dup2(old_stdout, STDOUT_FILENO);
	close(old_stdout);
	return (ret);
}

//this function is in a child process
static int	exec_child(t_node *cmd, t_data *data, int mode)
{
	int		out;
	int		in;
	int		res;
	char	*exec_path;

	out = 0;
	in = 0;
	if (cmd->right_child)
	{
		res = configure_redir(cmd->right_child, data, &in, &out);
		if (res < 0)
			exit(res);
	}
	if (mode != 4)
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
	if (!is_builtin(cmd->left_child->content.tab[0]))
	{
		exec_path = get_exe_path(data, cmd->left_child->content.tab[0]);
		if (exec_path == NULL)
			cmd_not_found(cmd->left_child->content.tab[0], data);
		if (execve(exec_path, cmd->left_child->content.tab, data->env) == -1)
			exec_fail(exec_path, cmd->left_child->content.tab[0], data);
	}
	else
	{
		res = run_builtins(cmd->left_child->content.tab, data, -2, -2);
		free_tree(data->tree);
		if (data->line != NULL)
			free(data->line);
		free_split(data->env);
		free(data->default_path);
		rl_clear_history();
		clean_data(data);
		exit(res);
	}
	exit(0);
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
		return (exec_builtin(cmd, data));
	}
	if (mode != 4)
	{
		data->child_cnt++;
		data->pid_tab[data->cmd_cnt - 1] = fork();
		if (data->pid_tab[data->cmd_cnt - 1] == -1)
			return (-4); //fork error
		if (data->pid_tab[data->cmd_cnt - 1] == 0)
		{
			handle_signals_child();
			exec_child(cmd, data, mode);
		}
		if (mode < 3) //in parent
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
			exec_child(cmd, data, mode);
		}
		return (0);
	}
}
