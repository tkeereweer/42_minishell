/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:59:37 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/08 09:42:10 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_path(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*temp1;
	char	*ret;

	i = 0;
	ret = NULL;
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
		{
			ret = ft_strdup(temp);
			if (access(temp, X_OK) == 0)
				return (free(temp1), free(ret), free_split(paths), temp);
		}
		free(temp);
		i++;
	}
	free(temp1);
	free_split(paths);
	return (ret);
}

//the + 1 in the substr is to include a / before the join
static char *find_local_path(t_data *data, char *cmd)
{
	char    *ret;
	char    *pwd;
	char    *curr_wd;
	int     i;

	if (!ft_strncmp(cmd, "./", 2))
	{
		pwd = ft_getenv("$PWD", data->env);
		if (!pwd)
			return (NULL);
		ret = ft_strjoin(pwd, &cmd[1]);
		if (!ret)
			return (NULL);
		return (ret);
	}
	i = 1;
	while (cmd[ft_strlen(cmd) - i] != '/')
		i++;
	pwd = ft_substr(cmd, 0, ft_strlen(cmd) - i + 1);
	if (!pwd)
		return (NULL);
	if (chdir(pwd) == -1)
		return (free(pwd), NULL);
	curr_wd = getcwd(NULL, 0);
	free(pwd);
	if (!curr_wd)
		return (NULL);
	ret = ft_strjoin(curr_wd, "/");
	if (!ret)
		return (NULL);
	curr_wd = ft_strjoin(ret, &cmd[ft_strlen(cmd) - i + 1]);
	return (free(ret), curr_wd);
}

char	*get_exe_path(t_data *data, char *cmd)
{
	char 	*env_path;
	char	**paths;

	if (!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "../", 3))
		return (find_local_path(data, cmd));
	if (cmd[0] == '\0')
		return (cmd);
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
	ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	free_tree(data->tree);
	free_split(data->env);
	if (data->default_path != NULL)
		free(data->default_path);
	rl_clear_history();
	clean_data(data);
	exit(127);
}

void	permission_error(char *path, t_data *data)
{
	ft_putstr_fd("minishell:", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	free_tree(data->tree);
	free_split(data->env);
	if (data->default_path != NULL)
		free(data->default_path);
	rl_clear_history();
	free(path);
	clean_data(data);
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
	if (data->default_path != NULL)
		free(data->default_path);
	rl_clear_history();
	free(path);
	clean_data(data);
	exit(1);
}
