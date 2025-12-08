/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:24:33 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/08 21:29:13 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void in_quotes_increment(char *str, int i, int *small, int *big)
{
	if (str[i] == '\'' && *small == 0 && *big == 0)
		*small += 1;
	else if (str[i] == '"' && *big == 0 && *small == 0)
		*big += 1;
	else if (str[i] == '\'' && *small != 0 && *big == 0)
		*small -= 1;
	else if (str[i] == '"' && *big != 0 && *small == 0)
		*big -= 1;
	return ;
}

int	in_quotes(char *str, int idx)
{
	int	i;
	int	big;
	int	small;

	i = 0;
	big = 0;
	small = 0;
	while (str[i] != '\0')
	{
		in_quotes_increment(str, i, &small, &big);
		if (i == idx && str[i] == '"' && small == 0)
			return (3);
		else if (i == idx && str[i] == '\'' && big == 0)
			return (3);
		if (i == idx && big == 0 && small == 0)
			return (0);
		i++;
	}
	return (0);
}

static void increment_i_and_j(size_t *i, size_t *j)
{
	*i += 1;
	*j += 1;
	return ;
}

static void	increment_j(size_t *i, size_t *j, int *start_i, int *match)
{
	*start_i = *j;
	*match = *i;
	*j += 1;
	return ;
}

static void	increment_match(size_t *i, size_t *j, int *start_i, int *match)
{
	*j = *start_i + 1;
	*match += 1;
	*i = *match;
	return ;
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
		if ((pat[j] == '\'' || pat[j] == '"') && in_quotes(pat, j) == 3)
			j++;
		else if (j < ft_strlen(pat) && pat[j] == str[i])
			increment_i_and_j(&i, &j);
		else if (j < ft_strlen(pat) && pat[j] == '*' && in_quotes(pat, j) == 0)
			increment_j(&i, &j, &start_i, &match);
		else if (start_i != -1)
			increment_match(&i, &j, &start_i, &match);
		else
			return (0);
	}
	while (j < ft_strlen(pat) && pat[j] == '*' && in_quotes(pat, j) == 0)
		j++;
	return (j == ft_strlen(pat));
}

static int  when_first_not_1(int *len, char ***tab)
{
	while ((*tab)[*len] != NULL)
		*len += 1;
	*tab = tab_realloc(*tab, *len + 2);
	if (*tab == NULL)
		return (1);
	return (0);
}

int	add_file(char ***tab, int first, int i, char *filename)
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

int	expand_wildcards(char ***tab, int i, char *pat)
{
	char			buf[PATH_MAX];
	DIR				*dir_stream;
	struct dirent	*dir_entry;
	int				first;

	if (has_wc(pat) == -1)
		return (0);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (-1);
	dir_stream = opendir(buf);
	if (dir_stream == NULL)
		return (-1);
	dir_entry = readdir(dir_stream);
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

int	add_to_redir_path(char **path, int first, char *filename)
{
	char	*new_path;

	if (first == 1)
		new_path = (char *) my_realloc(*path, (ft_strlen(filename) + 1) * sizeof(char));
	else
		new_path = (char *) my_realloc(*path, (ft_strlen(filename) + ft_strlen(*path) + 1) * sizeof(char));
	if (new_path == NULL)
		return (1);
	ft_strcat(new_path, filename);
	*path = new_path;
	return (0);
}

// static int	when_first_neg(DIR *dir_stream)
// {
// 	if (closedir(dir_stream) == -1)
// 		return (-1);
// 	return (-4); 
// }

// static int	iterate_dir_redir(DIR *dir_stream, struct dirent *dir_entry,
// 								char *pat, char **path)
// {
// 	int	first;

// 	first = 1;
// 	while (dir_entry != NULL)
// 	{
// 		if (match_pat(dir_entry->d_name, pat) == 1)
// 		{
// 			if (add_to_redir_path(path, first, dir_entry->d_name) == 1)
// 			{
// 				closedir(dir_stream);
// 				return (-1);
// 			}
// 			first--;
// 		}
// 		dir_entry = readdir(dir_stream);
// 	}
// 	if (first < 0)
// 		return (when_first_neg(dir_stream));
// 	return (0);
// }

int	expand_wildcards_redir(char **path, char *pat)
{
	char			buf[PATH_MAX];
	DIR				*dir_stream;
	struct dirent	*dir_entry;
	// int				ret;
	int				first;

	if (has_wc(pat) == -1)
		return (0);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (-1);
	dir_stream = opendir(buf);
	if (dir_stream == NULL)
		return (-1);
	dir_entry = readdir(dir_stream);
	// ret = iterate_dir_redir(dir_stream, dir_entry, pat, path);
	// if (ret < 0)
	// 	return (ret);
	first = 1;
	while (dir_entry != NULL)
	{
		if (match_pat(dir_entry->d_name, pat) == 1)
		{
			if (add_to_redir_path(path, first, dir_entry->d_name) == 1)
			{
				closedir(dir_stream);
				return (-1);
			}
			first--;
		}
		dir_entry = readdir(dir_stream);
	}
    if (first < 0) //ambig redirect
    {
        if (closedir(dir_stream) == -1)
            return (-1);
        return (-4);        
    }
	return (closedir(dir_stream));
}
	// if (first < 0)
	// 	return (when_first_neg(dir_stream));