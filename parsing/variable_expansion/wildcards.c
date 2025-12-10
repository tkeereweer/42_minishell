/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:24:33 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 11:49:28 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	when_first_not_1(int *len, char ***tab)
{
	while ((*tab)[*len] != NULL)
		*len += 1;
	*tab = tab_realloc(*tab, *len + 2);
	if (*tab == NULL)
		return (1);
	return (0);
}

static int	add_file(char ***tab, int first, int i, char *filename)
{
	int		len;
	char	*tmp1;
	char	*tmp2;

	len = 0;
	if (first != 1)
		if (when_first_not_1(&len, tab))
			return (1);
	tmp2 = (*tab)[i];
	(*tab)[i] = ft_strdup(filename);
	if ((*tab)[i] == NULL)
		return (1);
	if (first == 1)
		return (0);
	while (i < len)
	{
		tmp1 = (*tab)[i + 1];
		(*tab)[i + 1] = tmp2;
		tmp2 = tmp1;
		i++;
	}
	return (0);
}

static int	setup_expand_wildcards(char *pat, DIR **dir_stream,
	struct dirent **dir_entry)
{
	char	buf[PATH_MAX];

	if (has_wc(pat) == -1)
		return (0);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (-1);
	*dir_stream = opendir(buf);
	if (*dir_stream == NULL)
		return (-1);
	*dir_entry = readdir(*dir_stream);
	return (1);
}

int	expand_wildcards(char ***tab, int i, char *pat)
{
	DIR				*dir_stream;
	struct dirent	*dir_entry;
	int				ret;
	int				first;

	ret = setup_expand_wildcards(pat, &dir_stream, &dir_entry);
	if (ret < 1)
		return (ret);
	first = 1;
	while (dir_entry != NULL)
	{
		if (match_pat(dir_entry->d_name, pat) == 1)
		{
			if (add_file(tab, first, i, dir_entry->d_name) == 1)
			{
				closedir(dir_stream);
				return (-1);
			}
			first = 0;
			i++;
		}
		dir_entry = readdir(dir_stream);
	}
	return (closedir(dir_stream));
}
