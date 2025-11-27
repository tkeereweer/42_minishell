/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:36:14 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/27 14:34:32 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	empty_end(char *line, int *j, int *i)
{
	while (line[*i] && ft_is_whitespace(line[*i]))
		*i += 1;
	if (!line[*i])
		return (-1);//syntax error
	*j = *i;
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
	while (ft_is_whitespace(str[start  + i]) || str[start + i] == '<' || str[start + i] == '>')
		i++;
	dest_len = ft_strlen(str) - (end - start);
	dest = (char *)malloc(dest_len * sizeof(char));
	if (!dest)
		return (NULL);
	ft_strncpy(dest, str, start + i - 1);
	i = end + 1;
	j = start;
	while (ft_is_whitespace(str[i]))
		i++;   
	while (str[i])
	{
		dest[j] = str[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	return (dest);
}

t_list	*syntax_error(t_list **lst)
{
	int		temp;
	t_list *last;

	last = ft_lstlast(*lst);
	temp = last->content->content.logic;
	write(STDERR_FILENO, "syntax error near: ", ft_strlen("syntax error near: "));
	if (temp == 0)
		write(STDERR_FILENO, "'&&'\n", ft_strlen("'&&'\n"));
	else
		write(STDERR_FILENO, "'||'\n", ft_strlen("'||'\n"));
	ft_lstclear(lst, del_linked);
	return (NULL);
}
