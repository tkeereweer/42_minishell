/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exe_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:11:28 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 11:08:07 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*init_path(int *i, char **ret, char **temp1, char *cmd)
{
	*i = 0;
	*ret = NULL;
	*temp1 = ft_strjoin("/", cmd);
	if (!*temp1)
		return (NULL);
	return (cmd);
}

char	*find_path(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*temp1;
	char	*ret;

	if (cmd[0] == '/')
		return (ft_strdup(cmd));
	if (!init_path(&i, &ret, &temp1, cmd))
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
	return (free(temp1), free_split(paths), ret);
}

//the + 1 in the substr is to include a / before the join
static char	*find_local_path(t_data *data, char *cmd)
{
	char	*ret;
	char	*pwd;

	pwd = ft_getenv("$PWD", data->env);
	if (!pwd)
		return (NULL);
	ret = ft_strjoin(pwd, &cmd[1]);
	if (!ret)
		return (NULL);
	return (ret);
}

static char	*find_relative_path(char *cmd)
{
	int		i;
	char	*pwd;
	char	*curr_wd;
	char	*ret;

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
	char	*env_path;
	char	**paths;

	if (!ft_strncmp(cmd, "./", 2))
		return (find_local_path(data, cmd));
	if (!ft_strncmp(cmd, "../", 3))
		return (find_relative_path(cmd));
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
