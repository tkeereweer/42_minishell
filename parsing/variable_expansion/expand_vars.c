/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 10:50:02 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/27 09:55:55 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	remove_quotes(char **str)
{
	char	*tmp;

    //remove balanced quotes when inside quotes of the same type
    //echo 'this is 'abc' test' --> this is abc test
    //echo 'this is "abc" test' --> this is "abc" test
    //echo "abc"thsi'test' --> abcthsitest
    //echo "abcthsi''test" --> abcthsi''test
	tmp = *str;
	*str = ft_substr(*str, 1, ft_strlen(*str) - 2);
	if (*str == NULL)
		return (1);
	free(tmp);
	return (0);
}

int	expand_vars(char ***tab, t_data *data)
{
	int	i;
	int	mode;

	i = 1;
	while ((*tab)[i] != NULL)
	{
		mode = 0;
		if ((*tab)[i][0] == '\'')
			mode = 2;
		else if ((*tab)[i][0] == '"')
			mode = 1;
		if (mode == 1 || mode == 2)
		{
			if (remove_quotes(&(*tab)[i]) == 1)
				return (1);
		}
		if (mode != 2)
		{
			if (expand_envvars(&(*tab)[i], data) == 1)
				return (1);
		}
		if (mode != 1)
		{
			if (expand_wildcards(tab, i, (*tab)[i]) == 1)
				return (1);
		}
		i++;
	}
	return (0);
}

int	expand_vars_redir(char **path, t_data *data)
{
	int	i;
	int	mode;

    
	i = 1;
	mode = 0;
	if (*path[0] == '\'')
		mode = 2;
	else if (*path[0] == '"')
		mode = 1;
	if (mode == 1 || mode == 2)
	{
		if (remove_quotes(path) == 1)
			return (-1);
	}
	if (mode != 2)
	{
		if (expand_envvars(path, data) == 1)
			return (-1);
	}
	if (mode != 1)
	{
		if (expand_wildcards_redir(path, *path) == 1)
			return (-1);
	}
	i++;
	return (1);
}
