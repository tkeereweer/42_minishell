/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 09:21:20 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/03 15:03:18 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*str_realloc(char *ptr, size_t size)
{
	char	*out;

	out = (char *) malloc(size);
	if (out == NULL)
	{
		if (ptr)
			free(ptr);
		return (NULL);
	}
	if (ptr)
	{
		ft_strlcpy(out, ptr, size);
		free(ptr);
	}
	return (out);
}

int	update_env(char *old_pwd, t_data *data)
{
	char	*key_val;
	char	buf[PATH_MAX];
	int		ret;

	key_val = (char *) malloc((ft_strlen(old_pwd) + 8) * sizeof(char));
	if (key_val == NULL)
		return (-1);
	ft_strlcpy(key_val, "OLDPWD=", 8);
	ft_strcat(key_val, old_pwd);
	ret = ft_export(key_val, data);
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
	ret = ft_export(key_val, data);
	free(key_val);
	if (ret == 1)
		return (-1);
	return (0);
}

int	ft_cd(char *path, t_data *data)
{
	int		ret;
	char	*home;
	char	*new_str;
	char	buf[PATH_MAX];

	ret = -1;
	if (getcwd(buf, PATH_MAX) == NULL)
		return (-1);
	if (path == NULL)
	{
		home = getenv("HOME");
		// maybe still execute if HOME is empty
		if (home != NULL)
			ret = chdir(home);
	}
	// maybe not necessary to handle ~ expansion
	else if (path[0] == '~')
	{
		home = getenv("HOME");
		if (home != NULL)
		{
			new_str = (char *) malloc((ft_strlen(home) + ft_strlen(&path[1]) + 1) * sizeof(char));
			if (new_str == NULL)
				return (-1);
			ft_strlcpy(new_str, home, ft_strlen(home) + ft_strlen(&path[1]) + 1);
			ft_strlcat(new_str, &path[1], ft_strlen(home) + ft_strlen(&path[1]) + 1);
			ret = chdir(new_str);
			free(new_str);
		}
		else
			ret = chdir(&path[1]);
	}
	else
		ret = chdir(path);
	if (ret == 0)
		return (update_env(buf, data));
	if (ret == -1)
		perror("cd");
	return (ret);
}
