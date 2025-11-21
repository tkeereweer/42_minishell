/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:29:27 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/21 15:53:00 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// end++ is to offset the last quote when replacing w/ path
static char	*remove_quotes(char *line, int start, int *end)
{
	char	*dst;
	size_t	new_len;
	size_t	i;


	new_len = *end - start;
	dst = (char *)malloc(sizeof(char) * (new_len + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (i < new_len - 1)
	{
		dst[i] = line[start + i + 1];
		i++;
	}
	dst[i] = '\0';
	*end += 1;
	return (dst);
}

//start and end are indexes in the main line
//remove end - start + 1 and add ft_strlen(path)
static int	replace_with_path(char **dest, char *path, int start, int end)
{
	char    *temp;
	size_t  new_len;

	new_len = ft_strlen(*dest) - ( end - start + 1) + ft_strlen(path);
	temp = (char *)malloc(sizeof(char) * (new_len + 1));
	temp[new_len] = '\0';
	if (!temp)
		return (0);
	ft_strncat(temp, *dest, start);
	ft_strncat(temp, path, ft_strlen(path));
	ft_strncat(temp, &(*dest)[end], ft_strlen(&(*dest)[end]));
	free(*dest);
	*dest = temp;
	return (1);
}

static char	*set_limiter(char **line, int *j, int *start)
{
	char	*limiter;
	
	*j += 2;
	while (ft_is_whitespace((*line)[*j]))
		*j += 1;
	*start = *j;
	if (!iterate_over_quotes(*line, j))
		return (NULL);
	if (!((*line)[*j] == '\'' || (*line)[*j] == '"'))
		while (!ft_is_whitespace((*line)[*j]))
			*j += 1;
	if ((*line)[*j] == '\'' || (*line)[*j] == '"')
		limiter = remove_quotes(*line, *start, j);
	else
		limiter = ft_substr(*line, *start, *j - *start);
	if (!limiter)
		return (NULL);
	return (limiter);
}
int	set_heredoc(char **line, int *j, char **tab)
{
	int     start;
	char    *limiter;
	int     i;

	start = 0;
	limiter = set_limiter(line, j, &start);
	if (!limiter)
		return (0);
	tab = heredoc(tab, limiter);
	if (!tab)
		return (free(limiter), 0);
	i = 0;
	while (tab[i])
		i++;
	if (!replace_with_path(line, tab[i - 1], start, *j))
		return (free(limiter), -1);
	return (free(limiter), 1);
}
