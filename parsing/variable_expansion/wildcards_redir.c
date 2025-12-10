/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 11:37:44 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 11:39:38 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	add_to_redir_path(char **path, int first, char *filename)
{
	char	*new_path;

	if (first == 1)
		new_path = (char *) my_realloc(*path,
				(ft_strlen(filename) + 1) * sizeof(char));
	else
		new_path = (char *) my_realloc(*path,
				(ft_strlen(filename) + ft_strlen(*path) + 1) * sizeof(char));
	if (new_path == NULL)
		return (1);
	if (first == 1)
		ft_strlcpy(new_path, filename, ft_strlen(filename) + 1);
	else
	{
		ft_strcat(new_path, " ");
		ft_strcat(new_path, filename);
	}
	*path = new_path;
	return (0);
}

static int	when_first_neg(DIR *dir_stream)
{
	if (closedir(dir_stream) == -1)
		return (-1);
	return (-4);
}

static int	setup_expand_wildcards_redir(char **pat, char *pattern,
	DIR **dir_stream, struct dirent **dir_entry)
{
	char	buf[PATH_MAX];

	*pat = ft_strdup(pattern);
	if (*pat == NULL)
		return (-1);
	if (has_wc(*pat) == -1)
		return (free(*pat), 0);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (free(*pat), -1);
	*dir_stream = opendir(buf);
	if (*dir_stream == NULL)
		return (free(*pat), -1);
	*dir_entry = readdir(*dir_stream);
	return (1);
}

int	expand_wildcards_redir(char **path, char *patt)
{
	DIR				*dir_stream;
	struct dirent	*dir_entry;
	int				flag;
	char			*pat;

	flag = setup_expand_wildcards_redir(&pat, patt, &dir_stream, &dir_entry);
	if (flag < 1)
		return (flag);
	flag = 1;
	while (dir_entry != NULL)
	{
		if (match_pat(dir_entry->d_name, pat) == 1)
		{
			if (add_to_redir_path(path, flag, dir_entry->d_name) == 1)
			{
				closedir(dir_stream);
				return (free(pat), -1);
			}
			flag--;
		}
		dir_entry = readdir(dir_stream);
	}
	if (flag < 0)
		return (free(pat), when_first_neg(dir_stream));
	return (free(pat), closedir(dir_stream));
}
