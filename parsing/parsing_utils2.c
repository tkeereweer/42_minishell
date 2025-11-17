/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:36:14 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/17 16:20:22 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	empty_end(char *line, int *j, int *i)
{
	*j = *i;
	while (line[*j] && ft_is_whitespace(line[*j]))
		*j += 1;
	if (!line[*j])
		return (-1);//syntax error
	return (1);
}

void	free_split(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	subpipe_error(int code, char **subpipe)
{
	if (subpipe)
		free_split(subpipe);
	return (code);
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

//deletes chars from start to end INCLUDED 
//overwrites them with the end
char	*remove_redir(char *str, int start, int end)
{
	char	*dest;
	size_t	dest_len;
	int		i;
	int		j;

	dest_len = ft_strlen(str) - (start - end);
	dest = (char *)malloc(dest_len);
	if (!dest)
		return (NULL);
	ft_strncpy(dest, str, start);
	i = end;
	j = start;
	while (str[i])
	{
		dest[j] = str[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	free(str);
	return (dest);
}
