/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterate_over_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:36:14 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 11:15:05 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_char(char *str)
{
	if (!*str)
		return (0);
	if (*str == '|')
		return (0);
	if (is_logic(str))
		return (0);
	if (is_redir(str))
		return (0);
	if (ft_is_whitespace(*str))
		return (0);
	return (1);
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
		while (((small_quote % 2 != 0) || (big_quote % 2 != 0))
			|| valid_char(&line[*j + 1]))
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

int	empty_end(char *line, int *j, int *i)
{
	while (line[*i] && ft_is_whitespace(line[*i]))
		*i += 1;
	if (!line[*i])
		return (-1);
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

void	del_linked(t_node *lst)
{
	(void)lst;
}
