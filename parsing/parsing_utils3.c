/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 10:39:11 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/17 13:58:41 by mkeerewe         ###   ########.fr       */
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
	int	quote_count;
	int	word_count;
	char **tab;

	quote_count = 0;
	word_count = 0;
	i = 0;
	j = 0;
	while(str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote_count++;
			while (quote_count != 0)
			{
				i++;
				if (str[i] == '\'' || str[i] == '"')
					quote_count--;
			}
			i++;
		}
		if (ft_is_whitespace(str[i]))
		{
			word_count++;
			tab = tab_realloc(tab, word_count);
			if (!tab)
				return (free_split(tab), NULL);
			tab[word_count - 1] = ft_substr(str, j, i);
			if (!tab[word_count - 1])
				return (free_split(tab), NULL);
			while (ft_is_whitespace(str[i]))
				i++;
		}
	}
	return (tab);
}
