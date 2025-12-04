/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:59:37 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/04 13:54:04 by mkeerewe         ###   ########.fr       */
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

char	*get_exe_path(t_data *data, char *cmd)
{
	char 	*env_path;
	char	**paths;

	env_path = ft_getenv("$PATH", data->env);
	if (env_path == NULL)
		env_path = data->default_path;
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	return (find_path(paths, cmd));
}

void	cmd_not_found(char *cmd, t_data *data)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
	free_tree(data->tree);
	free_split(data->env);
	free(data->default_path);
	free(data->pid_tab);
	rl_clear_history();
	exit(127);
}

void	permission_error(char *path, t_data *data)
{
	ft_putstr_fd("minishell:", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	free_tree(data->tree);
	free_split(data->env);
	free(data->default_path);
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
	free(data->default_path);
	free(data->pid_tab);
	rl_clear_history();
	exit(1);
}
