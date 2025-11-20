/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:24:33 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/20 16:52:56 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	has_wc(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '*')
			return (i);
		i++;
	}
	return (-1);
}

int	match_pat(char *str, char *pat)
{
	size_t	i;
	size_t	j;
	int	start_i;
	int	match;

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

int	expand_wildcards(char ***tab, int i, char *pat)
{
	char			buf[PATH_MAX];
	DIR				*dir_stream;
	struct dirent	*dir_entry;
	int				first;

	if (has_wc(pat) == -1)
		return (0);
	getcwd(buf, PATH_MAX);
	dir_stream = opendir(buf);
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

int	remove_quotes(char **str)
{
	char	*tmp;

	tmp = *str;
	*str = ft_substr(*str, 1, ft_strlen(*str) - 2);
	if (*str == NULL)
		return (1);
	free(tmp);
	return (0);
}

int	expand_vars(char ***tab, t_data *data)
{
	int	i;
	int	mode;

	i = 1;
	while ((*tab)[i] != NULL)
	{
		if ((*tab)[i][0] == '\'')
			mode = 2;
		else if ((*tab)[i][0] == '"')
			mode = 1;
		if (mode == 1 || mode == 2)
		{
			if (remove_quotes(&(*tab)[i]) == 1)
				return (1);
		}
		if (mode != 2)
		{
			if (expand_envvars(&(*tab)[i], data) == 1)
				return (1);
		}
		if (mode != 1)
		{
			if (expand_wildcards(tab, i, (*tab)[i]) == 1)
				return (1);
		}
		i++;
	}
	return (0);
}
