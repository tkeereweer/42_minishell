/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:29:27 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/08 15:11:02 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_path_tab(char **path_tab)
{
	int i;

	if (!path_tab)
		return;
	i = 0;
	while(path_tab[i])
	{
		if (unlink(path_tab[i]) == -1)
		{
			ft_putstr_fd("failed to delete temp file: ", STDERR_FILENO);
			ft_putstr_fd(path_tab[i], STDERR_FILENO);
			write(STDERR_FILENO, "\n", 1);
		}
		free(path_tab[i]);
		path_tab[i] = NULL;
		i++;
	}
	free(path_tab);
	return;
}

//start and end are indexes in the main line
//remove end - start + 1 and add ft_strlen(path)
char	*replace_with_path(char *dest, char *path, int start, int end)
{
	char    *temp;
	size_t  new_len;

	new_len = ft_strlen(dest) - ( end - start + 1) + ft_strlen(path) + 1;
	temp = (char *)ft_calloc(sizeof(char), new_len + 1);
	if (!temp)
		return (NULL);
	ft_strncat(temp, dest, start);
	ft_strncat(temp, path, ft_strlen(path));
	ft_strncat(temp, &dest[end], ft_strlen(&dest[end]));
	return (temp);
}

static char *is_heredoc_quoted(char **line, int *j, int *quoted_heredoc, int *end)
{
	while (1)
	{
		if ((*line)[*j] == '\'' || (*line)[*j] == '"')
		{
			*quoted_heredoc = 1;
			if (!iterate_over_quotes(*line, j))
			{
				*quoted_heredoc = -1;
				return (NULL);
			}
			*j += 1;
			*end = *j - 1;
			break;
		}
		if (!valid_char(&(*line)[*j]))
		{
			*end = *j - 1;
			break;
		}
		*j += 1;
	}
	return ("");
}

//j++ to put back j on the index after last char prevent offset when
//rebuilding line w/ temp filepath
static char	*set_limiter(char **line, int *j, int *start, int *quoted_heredoc)
{
	char	*limiter;
	int		end;
	
	*j += 2;
	while (ft_is_whitespace((*line)[*j]))
		*j += 1;
	*start = *j;
	if (!is_heredoc_quoted(line, j, quoted_heredoc, &end))
		return (NULL);
	limiter = ft_substr(*line, *start, end - *start + 1);
	if (!limiter)
		return (NULL);
	if (*quoted_heredoc)
		if (remove_quotes(&limiter) == -1)
			return (NULL);//cleans up if needed but substr removes quotes
	return (limiter);
}

static int quoted_heredoc_error(int quoted_heredoc)
{
	if (quoted_heredoc == -1)
		return (0);
	return (-1);   
}

static int  heredoc_tab_len(char ***tab)
{
	int i;

	i = 0;
	while ((*tab)[i])
		i++;
	return (i);
}

int set_heredoc(char **line, int *j, char ***tab)
{
	int     start;
	char    *limiter;
	char    *temp;
	int     path_num;
	int     quoted_heredoc;

	start = 0;
	quoted_heredoc = 0;
	if (handle_signals_parent(2) == 1)
		return (-1);
	limiter = set_limiter(line, j, &start, &quoted_heredoc);
	if (!limiter)
		return (quoted_heredoc_error(quoted_heredoc));
	*tab = heredoc(*tab, limiter, quoted_heredoc);
	if (!tab)
		return (free(limiter), -1);
	path_num = heredoc_tab_len(tab);
	if (handle_signals_parent(0) == 1)
		return (-1);
	temp = replace_with_path(*line, (*tab)[path_num - 1], start, *j);
	if (!temp)
		return (free(limiter), -1);
	free(*line);
	*line = temp;
	return (free(limiter), 1);
}
