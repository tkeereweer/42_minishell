/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_filepath.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:25:36 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 11:58:16 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//function called when heredoc and valid limiter are found
//creates a temporary file in /temp/dev or /dev
//close the file descriptor so file offset resets
//stores filepaths in path_tab, initialized to NULL
char	**heredoc(char **path_tab, char *limiter, int quoted_heredoc
		, int *err_flag)
{
	static int	count = 0;
	int			fd;

	if (!path_tab)
		count = 0;
	if (!limiter)
		return (NULL);
	count++;
	path_tab = temp_filepath(path_tab, count, quoted_heredoc);
	if (!path_tab)
		return (NULL);
	path_tab = try_filepath(path_tab, count);
	if (!path_tab)
		return (free_split(path_tab), NULL);
	fd = open(path_tab[count - 1], O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		return (free_split(path_tab), NULL);
	*err_flag = write_heredoc(limiter, fd);
	if (*err_flag < 0)
		return (free_split(path_tab), NULL);
	return (path_tab);
}

char	**tab_realloc(char **tab, int n)
{
	char	**temp;
	int		i;

	temp = (char **)ft_calloc(n + 1, sizeof(char *));
	if (!temp)
		return (NULL);
	temp[n] = NULL;
	if (!tab)
		return (temp);
	i = 0;
	while (tab[i])
		i++;
	ft_memmove(temp, tab, i * sizeof(char *));
	free(tab);
	tab = temp;
	return (tab);
}

//start and end are indexes in the main line
//remove end - start + 1 and add ft_strlen(path)
char	*replace_with_path(char *dest, char *path, int start, int end)
{
	char	*temp;
	size_t	new_len;

	new_len = ft_strlen(dest) - (end - start + 1) + ft_strlen(path) + 1;
	temp = (char *)ft_calloc(sizeof(char), new_len + 1);
	if (!temp)
		return (NULL);
	ft_strncat(temp, dest, start);
	ft_strncat(temp, path, ft_strlen(path));
	ft_strncat(temp, &dest[end], ft_strlen(&dest[end]));
	return (temp);
}

//deletes chars from start to end INCLUDED 
//overwrites them with the end
//start and end are relative to the position of the redir in the original string
//strncpy is for beginning of string until removed part
//while loop is for after removed part until end of line
char	*remove_redir(char *str, int start, int end)
{
	char	*dest;
	size_t	dest_len;
	int		i;
	int		j;

	i = 0;
	while (ft_is_whitespace(str[start + i])
		|| str[start + i] == '<' || str[start + i] == '>')
		i++;
	dest_len = ft_strlen(str) - (end - start) + 2;
	dest = (char *)malloc(dest_len * sizeof(char));
	if (!dest)
		return (NULL);
	ft_strncpy(dest, str, start + i - 1);
	i = end + 1;
	j = start;
	while (ft_is_whitespace(str[i]))
		i++;
	dest[j] = ' ';
	j++;
	while (str[i])
		dest[j++] = str[i++];
	dest[j] = '\0';
	return (free(str), dest);
}

int	return_1_subpipe(char ***subpipe, char *line)
{
	*subpipe = tab_realloc(*subpipe, 1);
	if (!*subpipe)
		return (-1);
	(*subpipe)[0] = ft_strdup(line);
	(*subpipe)[1] = NULL;
	return (1);
}
