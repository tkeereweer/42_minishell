/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:21:16 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 11:18:56 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void dup_old_streams(int old_stdin, int old_stdout)
{
	dup2(old_stdin, STDIN_FILENO);
	close(old_stdin);
	dup2(old_stdout, STDOUT_FILENO);
	close(old_stdout);
}

static void exec_builtin_in_child(t_data *data, t_node *cmd)
{
	int res;

	res = run_builtins(cmd->left_child->content.tab, data, -2, -2);
	free_tree(data->tree);
	if (data->line != NULL)
		free(data->line);
	free_split(data->env);
	if (data->default_path != NULL)
		free(data->default_path);
	rl_clear_history();
	clean_data(data);
	exit(res);
}

static void exec_non_builtin(t_data *data, t_node *cmd)
{
	char    *exec_path;
	
	exec_path = get_exe_path(data, cmd->left_child->content.tab[0]);
	if (exec_path == NULL)
		cmd_not_found(cmd->left_child->content.tab[0], data);
	if (exec_path[0] == '\0')
		exit(0);
	if (execve(exec_path, cmd->left_child->content.tab, data->env) == -1)
		exec_fail(exec_path, cmd->left_child->content.tab[0], data);
}

static void config_pipes_modes_123(t_data *data, int in, int out, int mode)
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

//this function is in a child process
int	exec_child(t_node *cmd, t_data *data, int mode)
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
			return(redir_error(cmd->right_child->content.redir.path, 0));
		if (res == -3)
			return (redir_error("heredoc", 0));
        if (res == -1 && errno == EACCES)
            permission_error_fd(cmd, mode);
		if (res < 0)
			exit(res); //permission error
	}
	if (mode != 4)
		config_pipes_modes_123(data, in, out, mode);
	if (!is_builtin(cmd->left_child->content.tab[0]))
		exec_non_builtin(data, cmd);
	else
		exec_builtin_in_child(data, cmd);
	exit(0);
}
