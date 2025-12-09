/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:07:31 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 13:39:59 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**get_envvar_pointer(char *var, char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(var, env[i], ft_strlen(var)) == 0
			&& env[i][ft_strlen(var)] == '=')
			return (&env[i]);
		i++;
	}
	return (NULL);
}

int	get_key_len(char *key_val)
{
	int	i;
	int	idx;

	i = 0;
	idx = -1;
	while (key_val[i] != '\0')
	{
		if (key_val[i] == '=')
		{
			idx = i;
			break ;
		}
		i++;
	}
	if (idx == -1)
		return (i);
	return (idx);
}

int	valid_key(char *key, int key_len)
{
	int	i;

	i = 0;
	if (key[0] == '=' || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	while (key[i] != '\0' && i < key_len)
	{
		if (ft_isalnum(key[i]) == 0 && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	add_key(char *key_val, t_data *data)
{
	int		i;
	char	**temp;

	i = 0;
	while (data->env[i] != NULL)
		i++;
	temp = tab_realloc(data->env, i + 1);
	if (temp == NULL)
		return (1);
	data->env = temp;
	data->env[i] = ft_strdup(key_val);
	if (data->env[i] == NULL)
		return (1);
	data->env[i + 1] = NULL;
	return (0);
}

int	key_already_exists(char **curr_key_val, char *key_val)
{
	char	*tmp;

	tmp = *curr_key_val;
	*curr_key_val = ft_strdup(key_val);
	if (*curr_key_val == NULL)
		return (1);
	free(tmp);
	return (0);
}
