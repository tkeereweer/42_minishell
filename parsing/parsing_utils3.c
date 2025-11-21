/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 10:39:11 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/21 16:40:20 by mturgeon         ###   ########.fr       */
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

int	iterate_over_quotes(char *line, int *j)
{
	int	small_quote;
	int	big_quote;

	small_quote = 0;
	big_quote = 0;
	if (line[*j] == '\'' || line[*j] == '"')
	{
		if (line[*j] == '\'')
			small_quote++;
		if (line[*j] == '"')
			big_quote++;
		while ((small_quote % 2 != 0) || (big_quote % 2 != 0))
		{
			*j += 1;
			if (!line[*j])
				return (0);
			if (line[*j] == '\'' && (big_quote % 2 == 0))
				small_quote++;
			if (line[*j] == '"' && (small_quote % 2 == 0))
				big_quote++;
		}
	}
	return (1);
}
