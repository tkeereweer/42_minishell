/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 08:59:27 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 17:28:32 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	if_small_quote(int *i, char **str)
{
	*i += 1;
	while ((*str)[*i] != '\'')
		*i += 1;
	return ;
}

int	expand_env_in_big_quotes(char **str, int *i, t_data *data)
{
	*i += 1;
	if (expand_envvar_str(str, *i, data, 1) == 1)
		return (-1);
	while ((*str)[*i] != '"')
		*i += 1;
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	i = 0;
	while (*str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
		i++;
	}
	return (*str1 - *str2);
}
