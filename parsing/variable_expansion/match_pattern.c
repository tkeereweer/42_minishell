/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_pattern.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:40:59 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 11:03:35 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	increment_i_and_j(size_t *i, size_t *j)
{
	*i += 1;
	*j += 1;
	return ;
}

static void	increment_j(size_t *i, size_t *j, int *start_i, int *match)
{
	*start_i = *j;
	*match = *i;
	*j += 1;
	return ;
}

static void	increment_match(size_t *i, size_t *j, int *start_i, int *match)
{
	*j = *start_i + 1;
	*match += 1;
	*i = *match;
	return ;
}

int	match_pat(char *str, char *pat)
{
	size_t	i;
	size_t	j;
	int		start_i;
	int		match;

	i = 0;
	j = 0;
	start_i = -1;
	match = 0;
	while (i < ft_strlen(str))
	{
		if ((pat[j] == '\'' || pat[j] == '"') && in_quotes(pat, j) == 3)
			j++;
		else if (j < ft_strlen(pat) && pat[j] == str[i])
			increment_i_and_j(&i, &j);
		else if (j < ft_strlen(pat) && pat[j] == '*' && in_quotes(pat, j) == 0)
			increment_j(&i, &j, &start_i, &match);
		else if (start_i != -1)
			increment_match(&i, &j, &start_i, &match);
		else
			return (0);
	}
	while (j < ft_strlen(pat) && pat[j] == '*' && in_quotes(pat, j) == 0)
		j++;
	return (j == ft_strlen(pat));
}
