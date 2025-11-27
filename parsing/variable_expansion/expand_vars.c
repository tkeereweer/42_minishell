/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 10:50:02 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/27 10:48:09 by mkeerewe         ###   ########.fr       */
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

int	clean_quotes(char **str, int i, char type)
{
	ft_strlcpy(&*str[i], &*str[i + 1], ft_strlen(*str) + 1);
	i++;
	while (*str[i] != type)
		i++;
	ft_strlcpy(&*str[i], &*str[i + 1], ft_strlen(*str) + 1);
	return (i);
}

int	new_remove_quotes(char **str)
{
	int	i;

	i = 0;
	while (*str[i] != '\0')
	{
		if (*str[i] == '\'' || *str[i] == '"')
			i = clean_quotes(str, i, *str[i]);
		i++;
	}
	*str = my_realloc(*str, ft_strlen(*str));
	if (*str == NULL)
		return (1);
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
			if (new_remove_quotes(&(*tab)[i]) == 1)
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
