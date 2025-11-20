/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:24:33 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/19 18:18:19 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	has_wc(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '*')
			return (i);
		i++;
	}
	return (-1);
}

int	expand_wildcards(char **str, t_data *data)
{
	int	wc_pos;
	// struct dirent	dir_entry;
	DIR				*dir_stream;

	wc_pos = has_wc(*str);
	if (wc_pos == -1)
		return (0);
	dir_stream = opendir())
}
