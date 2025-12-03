/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 12:16:15 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/03 18:16:50 by mturgeon         ###   ########.fr       */
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
	if (idx == 0)
		return (-2);
	else if (idx == -1)
		return (-1);
	return (idx);
}

int	add_key(char *key_val, t_data *data)
{
	int		i;
    char    **temp;

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

int	ft_export(char *key_val, t_data *data)
{
	char	*key;
	int		key_len;
	char	**curr_key_val;

	key_len = get_key_len(key_val);
	if (key_len == -1)
		return (1); // set errno ??
	else if (key_len == -2)
	{
		ft_printf("export: `%s': not a valid identifier", key_val);
		return (1); // set errno ??
	}
	key = ft_substr(key_val, 0, key_len);
	if (key == NULL)
		return (1);
	curr_key_val = get_envvar_pointer(key, data->env);
	free(key);
	if (curr_key_val != NULL)
		return (key_already_exists(curr_key_val, key_val));
	return (add_key(key_val, data));
}
