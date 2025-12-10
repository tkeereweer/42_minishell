/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:37:27 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 11:47:11 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	in_quotes_increment(char *str, int i, int *small, int *big)
{
	if (str[i] == '\'' && *small == 0 && *big == 0)
		*small += 1;
	else if (str[i] == '"' && *big == 0 && *small == 0)
		*big += 1;
	else if (str[i] == '\'' && *small != 0 && *big == 0)
		*small -= 1;
	else if (str[i] == '"' && *big != 0 && *small == 0)
		*big -= 1;
	return ;
}

int	in_quotes(char *str, int idx)
{
	int	i;
	int	big;
	int	small;

	i = 0;
	big = 0;
	small = 0;
	while (str[i] != '\0')
	{
		in_quotes_increment(str, i, &small, &big);
		if (i == idx && str[i] == '"' && small == 0)
			return (3);
		else if (i == idx && str[i] == '\'' && big == 0)
			return (3);
		if (i == idx && big == 0 && small == 0)
			return (0);
		i++;
	}
	return (0);
}

int	has_wc(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '*' && in_quotes(str, i) == 0)
			return (i);
		i++;
	}
	return (-1);
}
