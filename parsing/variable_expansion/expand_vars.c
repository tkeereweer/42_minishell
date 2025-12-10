/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 10:50:02 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 11:46:11 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	clean_quotes(char *str, int i, char type)
{
	if (str[i + 1] == type)
	{
		ft_strncpy(&str[i], &str[i + 2], ft_strlen(&str[i + 2]));
		return (i - 1);
	}
	ft_strncpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]));
	if (str[i + 1] != '\0')
		i++;
	while (str[i] != type)
		i++;
	ft_strncpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]));
	return (i - 1);
}

int	remove_quotes(char **str)
{
	int	i;

	i = 0;
	while ((*str)[i] != '\0')
	{
		if ((*str)[i] == '\'' || (*str)[i] == '"')
			i = clean_quotes(*str, i, (*str)[i]);
		i++;
	}
	*str = my_realloc(*str, ft_strlen(*str) + 1);
	if (*str == NULL)
		return (1);
	return (0);
}

int	expand_vars(char ***tab, t_data *data)
{
	int	i;

	i = 0;
	while ((*tab)[i] != NULL)
	{
		if (expand_envvars(&(*tab)[i], data) == -1)
			return (-1);
		if (expand_wildcards(tab, i, (*tab)[i]) == -1)
			return (-1);
		if (remove_quotes(&(*tab)[i]) == 1)
			return (-1);
		i++;
	}
	return (0);
}

int	expand_vars_redir(char **path, t_data *data)
{
	int	res;

	res = expand_envvars_redir(path, data);
	if (res < 0)
		return (res);
	res = expand_wildcards_redir(path, *path);
	if (res < 0)
		return (res);
	if (remove_quotes(path) == 1)
		return (-1);
	return (0);
}
