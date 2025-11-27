/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:24:33 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/27 16:22:04 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	do_ign(int *ign, int idx)
{
	int	i;

	i = 0;
	while (ign[i] != -1)
	{
		if (ign[i] == idx)
			return (1);
		i++;
	}
	return (0);
}

int	has_wc(char *str, int *ign)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '*' && do_ign(ign, i) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	match_pat(char *str, char *pat)
{
	size_t	i;
	size_t	j;
	int		start_i;
	int		match;

	i = 0;
	j = 0;
	start_i = -1;
	match = 0;
	while (i < ft_strlen(str))
	{
		if (j < ft_strlen(pat) && pat[j] == str[i])
		{
			i++;
			j++;
		}
		else if (j < ft_strlen(pat) && pat[j] == '*')
		{
			start_i = j;
			match = i;
			j++;
		}
		else if (start_i != -1)
		{
			j = start_i + 1;
			match++;
			i = match;
		}
		else
			return (0);
	}
	while (j < ft_strlen(pat) && pat[j] == '*')
		j++;
	return (j == ft_strlen(pat));
}

int	add_file(char ***tab, int first, int i, char *filename)
{
	int		len;
	char	*tmp1;
	char	*tmp2;

	tmp2 = (*tab)[i];
	(*tab)[i] = ft_strdup(filename);
	if ((*tab)[i] == NULL)
		return (1);
	if (first == 1)
		return (0);
	len = 0;
	while ((*tab)[len] != NULL)
		len++;
	*tab = tab_realloc(*tab, len + 2);
	if (*tab == NULL)
		return (1);
	while (i < len)
	{
		tmp1 = (*tab)[i + 1];
		(*tab)[i + 1] = tmp2;
		tmp2 = tmp1;
		i++;
	}
	return (0);
}

int	expand_wildcards(char ***tab, int i, char *pat, int *ign)
{
	char			buf[PATH_MAX];
	DIR				*dir_stream;
	struct dirent	*dir_entry;
	int				first;

	if (has_wc(pat, ign) == -1)
		return (0);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (1);
	dir_stream = opendir(buf);
	if (dir_stream == NULL)
		return (1);
	dir_entry = readdir(dir_stream);
	first = 1;
	while (dir_entry != NULL)
	{
		if (match_pat(dir_entry->d_name, pat) == 1)
		{
			if (add_file(tab, first, i, dir_entry->d_name) == 1)
				return (1); // handle error
			first = 0;
			i++;
		}
		dir_entry = readdir(dir_stream);
	}
	return (closedir(dir_stream));
}

int	add_to_redir_path(char **path, int first, char *filename)
{
	char *new_path;

	if (first == 1)
		new_path = (char *) my_realloc(*path, (ft_strlen(filename) + 1) * sizeof(char));
	else
		new_path = (char *) my_realloc(*path, (ft_strlen(filename) + ft_strlen(*path) + 1) * sizeof(char));
	if (new_path == NULL)
		return (1);
	strcat(new_path, filename);
	*path = new_path;
	return (0);
}

int	expand_wildcards_redir(char **path, char *pat, int *ign)
{
	char			buf[PATH_MAX];
	DIR				*dir_stream;
	struct dirent	*dir_entry;
	int				first;

	if (has_wc(pat, ign) == -1)
		return (0);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (1);
	dir_stream = opendir(buf);
	if (dir_stream == NULL)
		return (1);
	dir_entry = readdir(dir_stream);
	first = 1;
	while (dir_entry != NULL)
	{
		if (match_pat(dir_entry->d_name, pat) == 1)
		{
			if (add_to_redir_path(path, first, dir_entry->d_name)== 1)
				return (1); // handle error
			first = 0;
		}
		dir_entry = readdir(dir_stream);
	}
	return (closedir(dir_stream));
}
