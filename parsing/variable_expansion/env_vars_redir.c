/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 11:40:44 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 11:48:22 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_ambiguous(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\'' && str[i] != '"')
	{
		if (ft_is_whitespace(str[i]))
			return (-1);
		i++;
	}
	return (0);
}

int	expand_envvars_redir(char **str, t_data *data)
{
	int		i;

	i = 0;
	while ((*str)[i] != '\0')
	{
		if ((*str)[i] == '\'')
			if_small_quote(&i, str);
		else if ((*str)[i] == '"')
		{
			if (expand_env_in_big_quotes(str, &i, data) == -1)
				return (-1);
		}
		else if ((*str)[i] == '$')
		{
			if (expand_envvar_str(str, i, data, 0) == 1)
				return (-1);
			if (is_ambiguous(&(*str)[i]) < 0)
				return (-4);
			if ((*str)[0] != '\0' && envvar_len(&(*str)[i]) == 1)
				i++;
			continue ;
		}
		i++;
	}
	return (0);
}
