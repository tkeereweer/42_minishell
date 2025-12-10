/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exe_path_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:07:48 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 10:09:00 by mkeerewe         ###   ########.fr       */
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
