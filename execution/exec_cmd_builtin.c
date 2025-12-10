/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:03:47 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 10:05:25 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//need read end of previous for stdin and write end of current for stdout
int	exec_builtin(t_node *cmd, t_data *data, int mode)
{
	int	out;
	int	in;
	int	old_stdin;
	int	old_stdout;
	int	ret;

	out = 0;
	in = 0;
	if (setup_oldstds(&old_stdin, &old_stdout) == -1)
		return (-1);
	if (cmd->right_child)
	{
		ret = configure_redir(cmd->right_child, data, &in, &out);
		if (ret == -2)
			return (redir_error(cmd->right_child->content.redir.path, mode));
		if (ret == -1 && errno == EACCES)
			return (permission_error_fd(cmd, mode));
		if (ret < 0)
			return (1);
	}
	ret = run_builtins(cmd->left_child->content.tab, data,
			old_stdin, old_stdout);
	dup_old_streams(old_stdin, old_stdout);
	return (ret);
}

void	exec_builtin_in_child(t_data *data, t_node *cmd)
{
	int	res;

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
