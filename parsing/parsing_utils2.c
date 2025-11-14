/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:36:14 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/13 14:36:29 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	empty_end(char *line, int *j, int *i)
{
	*j = *i;
	while (line[*j] && ft_is_whitespace(line[*j]))
		j++;
	if (!line[*j])
		return (tokenizer_error("syntax error: no filepath or limiter\n"));
	return (1);
}