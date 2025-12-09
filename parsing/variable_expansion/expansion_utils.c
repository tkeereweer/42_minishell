/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:25:43 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 09:02:14 by mturgeon         ###   ########.fr       */
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
		if (ft_strncmp(var, env[i], ft_strlen(var)) == 0 && env[i][ft_strlen(var)] == '=')
			return (&env[i][ft_strlen(var) + 1]);
		i++;
	}
	return (NULL);
}

int	has_envvar(char *str, int i, int mode)
{
	if (mode == 0)
	{
		while (str[i] != '\0' && str[i] != '"')
		{
			if (str[i] == '$')
				return (i);
			i++;
		}
	}
	else
	{
		while (str[i] != '\0')
		{
			if (str[i] == '$')
				return (i);
			i++;
		}		
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
int is_ambiguous(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] != '\'' && str[i] != '"')
	{
		if (ft_is_whitespace(str[i]))
			return (-1);
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
