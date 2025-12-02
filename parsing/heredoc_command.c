/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:29:27 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/02 19:40:00 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	clean_path_tab(char **path_tab)
{
	int i;

	if (!path_tab)
		return (1);
	i = 0;
	while(path_tab[i])
	{
		if (unlink(path_tab[i]) == -1)
			printf("unlink fail\n");
		free(path_tab[i]);
		path_tab[i] = NULL;
		i++;
	}
	return (1);
}

// end++ is to offset the last quote when replacing w/ path
// static char	*remove_quotes(char *line, int start, int *end)
// {
// 	char	*dst;
// 	size_t	new_len;
// 	size_t	i;


// 	new_len = *end - start;
// 	dst = (char *)malloc(sizeof(char) * (new_len + 1));
// 	if (!dst)
// 		return (NULL);
// 	i = 0;
// 	while (i < new_len - 1)
// 	{
// 		dst[i] = line[start + i + 1];
// 		i++;
// 	}
// 	dst[i] = '\0';
// 	*end += 1;
// 	return (dst);
// }

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
			end = *j - 1;
			break;
		}
		if (!valid_char(&(*line)[*j]))
		{
			end = *j  - 1;
			break;
		}
		*j += 1;
	}
	limiter = ft_substr(*line, *start, end - *start + 1);
	if (!limiter)
		return (NULL);
	if (*quoted_heredoc)
		if (remove_quotes(&limiter) == -1)
			return (NULL);//cleans up if needed but substr removes quotes
	return (limiter);
}

int set_heredoc(char **line, int *j, char ***tab)
{
	int     start;
	char    *limiter;
	int     i;
	int     quoted_heredoc;

	start = 0;
	quoted_heredoc = 0;
	limiter = set_limiter(line, j, &start, &quoted_heredoc);
	if (!limiter)
	{
		if (quoted_heredoc == -1)
			return (0); //unclosed quotes
		return (-1);
	}
	*tab = heredoc(*tab, limiter, quoted_heredoc);
	if (!tab)
		return (free(limiter), -1);
	i = 0;
	while ((*tab)[i])
		i++;
	*line = replace_with_path(*line, (*tab)[i - 1], start, *j);
	if (!*line)
		return (free(limiter), -1);
	return (free(limiter), 1);
}
