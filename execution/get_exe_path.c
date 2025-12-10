/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exe_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:11:28 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 13:14:22 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*find_path(char **paths, char *cmd)
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

static char	*keep_cwd(char **wd, char *cmd, int *i)
{
	char	*pwd;

	*wd = getcwd(NULL, 0);
	if (!*wd)
		return (NULL);
	*i = 1;
	while (cmd[ft_strlen(cmd) - *i] != '/')
		*i += 1;
	pwd = ft_substr(cmd, 0, ft_strlen(cmd) - *i + 1);
	if (!pwd)
		return (NULL);
	return (pwd);
}

static char	*find_relative_path(char *cmd, int *err_flag)
{
	int		i;
	char	*pwd;
	char	*curr_wd;
	char	*ret;
	char	*wd;

	pwd = keep_cwd(&wd, cmd, &i);
	if (!pwd)
		return (NULL);
	if (chdir(pwd) == -1)
		return (pwd);
	curr_wd = getcwd(NULL, 0);
	if (!curr_wd)
		return (free(pwd), NULL);
	if (chdir(wd) == -1)
		return (free(pwd), free(curr_wd), free(wd), NULL);
	ret = ft_strjoin(curr_wd, "/");
	if (!ret)
		return (free(wd), free(pwd), NULL);
	free(curr_wd);
	curr_wd = ft_strjoin(ret, &cmd[ft_strlen(cmd) - i + 1]);
	if (is_directory(curr_wd) == 1)
		set_errno_isdir(err_flag);
	return (free(wd), free(pwd), free(ret), curr_wd);
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
