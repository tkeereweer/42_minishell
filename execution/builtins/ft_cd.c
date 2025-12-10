/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 09:21:20 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 10:20:09 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	update_env(char *old_pwd, t_data *data)
{
	char	*key_val;
	char	buf[PATH_MAX];
	int		ret;

	key_val = (char *) malloc((ft_strlen(old_pwd) + 8) * sizeof(char));
	if (key_val == NULL)
		return (-1);
	ft_strlcpy(key_val, "OLDPWD=", 8);
	ft_strcat(key_val, old_pwd);
	ret = ft_export_var(key_val, data);
	free(key_val);
	if (ret == 1)
		return (-1);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (-1);
	key_val = malloc((ft_strlen(buf) + 5) * sizeof(char));
	if (key_val == NULL)
		return (-1);
	ft_strlcpy(key_val, "PWD=", 5);
	ft_strcat(key_val, buf);
	ret = ft_export_var(key_val, data);
	free(key_val);
	if (ret == 1)
		return (-1);
	return (0);
}

static int	tilde_case(char **home, int *ret, char *path)
{
	char	*new_str;

	*home = getenv("HOME");
	if (*home != NULL)
	{
		new_str = (char *) malloc((ft_strlen(*home)
					+ ft_strlen(&path[1]) + 1) * sizeof(char));
		if (new_str == NULL)
			return (-1);
		ft_strlcpy(new_str, *home, ft_strlen(*home) + ft_strlen(&path[1]) + 1);
		ft_strlcat(new_str, &path[1], ft_strlen(*home)
			+ ft_strlen(&path[1]) + 1);
		*ret = chdir(new_str);
		free(new_str);
	}
	else
		*ret = chdir(&path[1]);
	return (1);
}

static int	set_home(char **home, char *path, int *ret)
{
	if (path == NULL)
	{
		*home = getenv("HOME");
		if (*home != NULL)
			*ret = chdir(*home);
	}
	else if (path[0] == '~')
	{
		if (tilde_case(home, ret, path) == -1)
			return (-1);
	}
	else
		*ret = chdir(path);
	return (1);
}

int	ft_cd(char *path, t_data *data)
{
	int		ret;
	char	*home;
	char	buf[PATH_MAX];

	ret = -1;
	if (getcwd(buf, PATH_MAX) == NULL)
		return (-1);
	if (set_home(&home, path, &ret) == -1)
		return (-1);
	if (ret == 0)
		return (update_env(buf, data));
	if (ret == -1)
	{
		write(STDERR_FILENO, "minishell: cd:", 11);
		perror(path);
	}
	return (ret);
}
