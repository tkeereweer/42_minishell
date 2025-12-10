/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exe_path_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:07:48 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 13:14:41 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*init_path(int *i, char **ret, char **temp1, char *cmd)
{
	*i = 0;
	*ret = NULL;
	*temp1 = ft_strjoin("/", cmd);
	if (!*temp1)
		return (NULL);
	return (cmd);
}

int	is_directory(char *path)
{
	struct stat	file;

	if (lstat(path, &file) == 0)
	{
		if (S_ISDIR(file.st_mode))
			return (1);
		return (0);
	}
	return (-1);
}

int	has_a_slash(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

void	set_errno_isdir(int *err_flag)
{
	errno = EISDIR;
	*err_flag = 1;
}
