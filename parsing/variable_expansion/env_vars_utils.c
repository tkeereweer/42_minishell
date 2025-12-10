/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:25:43 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 11:44:15 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_getenv(char *var, char **env)
{
	int	i;

	i = 0;
	var++;
	while (env[i] != NULL)
	{
		if (ft_strncmp(var, env[i], ft_strlen(var)) == 0
			&& env[i][ft_strlen(var)] == '=')
			return (&env[i][ft_strlen(var) + 1]);
		i++;
	}
	return (NULL);
}

static int	do_continue(char c, int mode)
{
	if (mode == 0 && (c != '\0' && c != '"' && c != '\''))
		return (1);
	else if (mode == 1 && (c != '\0' && c != '"'))
		return (1);
	else if (mode == 2 && c != '\0')
		return (1);
	else
		return (0);
}

int	has_envvar(char *str, int i, int mode)
{
	while (do_continue(str[i], mode) == 1)
	{
		if (str[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

int	envvar_len(char *str)
{
	int	i;

	i = 1;
	if (str[1] == '?')
		return (i + 1);
	while (str[i] != '\0' && (ft_isalnum(str[i]) == 1 || str[i] == '_'))
		i++;
	return (i);
}

void	if_small_quote(int *i, char **str)
{
	*i += 1;
	while ((*str)[*i] != '\'')
		*i += 1;
	return ;
}
