/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:29:27 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/01 19:09:26 by mturgeon         ###   ########.fr       */
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
	// free(dest);
	return (temp);
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
		while ((*line)[*j] && !ft_is_whitespace((*line)[*j]))
			*j += 1;
	if ((*line)[*j] == '\'' || (*line)[*j] == '"')
		limiter = remove_quotes(*line, *start, j);
	else
		limiter = ft_substr(*line, *start, *j - *start);
	if (!limiter)
		return (NULL);
	return (limiter);
}
char	*set_heredoc(char *line, int *j, char ***tab)
{
	int     start;
	char    *limiter;
	int     i;

	start = 0;
	limiter = set_limiter(&line, j, &start);
	if (!limiter)
		return (NULL);
	*tab = heredoc(*tab, limiter);
	if (!tab)
		return (free(limiter), NULL);
	i = 0;
	while ((*tab)[i])
		i++;
	line = replace_with_path(line, (*tab)[i - 1], start, *j);
    if (!*line)
		return (free(limiter), NULL);
	return (free(limiter), line);
}
