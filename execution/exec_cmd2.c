/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:59:37 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/01 17:08:03 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_path(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*temp1;

	i = 0;
	if (cmd[0] == '/')
		return (ft_strdup(cmd));
	temp1 = ft_strjoin("/", cmd);
	if (!temp1)
		return (NULL);
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], temp1);
		if (!temp)
			return (free(temp1), free_split(paths), NULL);
		if (access(temp, F_OK) == 0)
			return (free(temp1), free_split(paths), temp);
		free(temp);
		i++;
	}
	free(temp1);
	free_split(paths);
	return (NULL);
}

char	*get_exe_path(char **env, char *cmd)
{
	char	**paths;

	paths = ft_split(ft_getenv("$PATH", env), ':');
	if (!paths)
		return (NULL);
	return (find_path(paths, cmd));
}

void	cmd_not_found(char *cmd, t_data *data)
{
	ft_printf("minishell: command not found: %s\n", cmd);
	free_tree(data->tree);
	free_split(data->env);
	free(data->pid_tab);
	rl_clear_history();
	exit(127);
}

void	permission_error(char *path, t_data *data)
{
	ft_printf("minishell: %s: Permission denied\n", path);
	free_tree(data->tree);
	free_split(data->env);
	free(data->pid_tab);
	rl_clear_history();
	exit(126);
}

void	exec_fail(char *path, char *cmd, t_data *data)
{
	if (errno == EACCES || errno == EISDIR)
	{
		permission_error(path, data);
	}
	perror(cmd);
	free_tree(data->tree);
	free_split(data->env);
	free(data->pid_tab);
	rl_clear_history();
	exit(1);
}
