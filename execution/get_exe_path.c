/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exe_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:11:28 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 08:42:50 by mturgeon         ###   ########.fr       */
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

static int  is_directory(char *path)
{
	struct stat file;

	if (lstat(path, &file) == 0)
	{
		if (S_ISDIR(file.st_mode))
			return (1);
		return (0);
	}
	return (-1);
}

char	*find_path(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*temp1;
	char	*ret;

	if (!ft_strncmp("..", cmd, ft_strlen(cmd)))
		return (NULL);
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

static char *find_relative_path(char *cmd, int *err_flag)
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
		return (pwd);
	curr_wd = getcwd(NULL, 0);
	free(pwd);
	if (!curr_wd)
		return (NULL);
	ret = ft_strjoin(curr_wd, "/");
	if (!ret)
		return (NULL);
    free(curr_wd);
	curr_wd = ft_strjoin(ret, &cmd[ft_strlen(cmd) - i + 1]);
	if (is_directory(curr_wd) == 1)
	{
		errno = EISDIR;
		*err_flag = 1;
	}
	return (free(ret), curr_wd);
}

int has_a_slash(char *cmd)
{
	int i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

char	*get_exe_path(t_data *data, char *cmd, int *err_flag)
{
	char	*env_path;
	char	**paths;

	if (has_a_slash(cmd))
		return (find_relative_path(cmd, err_flag));
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
