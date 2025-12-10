/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:29:27 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 11:58:12 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*is_heredoc_quoted(char **line, int *j,
		int *quoted_heredoc, int *end)
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
			break ;
		}
		if (!valid_char(&(*line)[*j]))
		{
			*end = *j - 1;
			break ;
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
			return (NULL);
	return (limiter);
}

static int	heredoc_tab_len(char ***tab)
{
	int	i;

	i = 0;
	while ((*tab)[i])
		i++;
	return (i);
}

static int	overwrite_path(char **line, int *j, char ***tab, int start)
{
	int		path_num;
	char	*temp;

	path_num = heredoc_tab_len(tab);
	temp = replace_with_path(*line, (*tab)[path_num - 1], start, *j);
	if (!temp)
		return (-1);
	free(*line);
	*line = temp;
	return (1);
}

int	set_heredoc(char **line, int *j, char ***tab)
{
	int		start;
	char	*limiter;
	int		quoted_heredoc;
	int		err_flag;

	start = 0;
	quoted_heredoc = 0;
	limiter = set_limiter(line, j, &start, &quoted_heredoc);
	if (!limiter)
		return (quoted_heredoc_error(quoted_heredoc));
	*tab = heredoc(*tab, limiter, quoted_heredoc, &err_flag);
	if (!*tab)
		return (free(limiter), err_flag);
	return (free(limiter), overwrite_path(line, j, tab, start));
}
