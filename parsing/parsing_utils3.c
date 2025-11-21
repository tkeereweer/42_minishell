/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 10:39:11 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/21 11:31:45 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tab_len(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**args_tab(char *str)
{
	int	i;
	int j;
	int small_quote;
    int big_quote;
	int	word_count;
	char **tab;
	char    *temp;

	tab = NULL;
	small_quote = 0;
    big_quote = 0;
	word_count = 0;
	i = 0;
	j = 0;
	tab = NULL;
	while(str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (str[i] == '\'')
				small_quote++;
			if (str[i] == '"')
				big_quote++;
			while ((small_quote % 2 != 0) || (big_quote % 2 != 0))
			{
				i++;
				if (str[i] == '\'' || str[i] == '"')
				{
					if (str[i] == '\'' && (big_quote % 2 == 0))
						small_quote++;
					if (str[i] == '"' && (small_quote % 2 == 0))
						big_quote++;
				}
            }
			// i++;
		}
		if (ft_is_whitespace(str[i]))
		{
			word_count++;
			tab = tab_realloc(tab, word_count);
			if (!tab)
				return (free_split(tab), NULL);
			temp = ft_substr(str, j, i - j);
			if (!temp)
				return (free_split(tab), NULL);
			tab[word_count - 1] = ft_strtrim(temp, " \t\n\v\f\r");
			if (!tab[word_count - 1])
				return (free(temp), free_split(tab), NULL);
			free(temp);
			while (ft_is_whitespace(str[i]))
				i++;
			j = i;
			continue;
		}
		i++;
	}
	if (!tab)
	{
		tab = tab_realloc(tab, 1);
		if (!tab)
			return (NULL);
		temp = ft_strdup(str);
		if (!temp)
			return (NULL);
		tab[0] = ft_strtrim(temp, " \t\n\v\f\r");
		if (!tab[0])
			return (free(temp), NULL);
		free(temp);
		tab[1] = NULL;
		return (tab);
	}
	tab = tab_realloc(tab, word_count + 1);
	if (!tab)
		return(free_split(tab), NULL);
	temp = ft_substr(str, j, i);
	if (!temp)
		return (free_split(tab), NULL);
	//this prevents calling ft_strtrim when temp is empty.
	if (temp[0] == '\0')
	{
		free(temp);
		tab[word_count] = NULL;
		return (tab);
	}
	tab[word_count] = ft_strtrim(temp, " \t\n\v\f\r");
	if (!tab[word_count])
		return (free(temp), free_split(tab), NULL);
	free(temp);
	return (tab);
}
