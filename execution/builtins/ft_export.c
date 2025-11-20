/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 12:16:15 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/20 09:29:31 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**get_envvar_pointer(char *var, char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(var, env[i], ft_strlen(var)) == 0 && env[i][ft_strlen(var)] == '=')
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
		return (-2); // bash: export: `=yoyo': not a valid identifier
	else if (idx == -1)
		return (-1);
	return (idx);
}

int	add_key(char *key_val, t_data *data)
{
	int		i;

	i = 0;
	while (data->env[i] != NULL)
		i++;
	data->env = tab_realloc(data->env, i + 1);
	if (data->env == NULL)
		return (1);
	data->env[i] = ft_strdup(key_val);
	if (data->env[i] == NULL)
		return (1); // also free tab??
	data->env[i + 1] = NULL;
	return(0);
}

int	ft_export(char *key_val, t_data *data)
{
	char	*key;
	int		key_len;
	char	*tmp;
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
	curr_key_val = get_envvar_pointer(key, data->env);
	if (curr_key_val != NULL)
	{
		tmp = *curr_key_val;
		*curr_key_val = ft_strdup(key_val);
		if (*curr_key_val == NULL)
			return (1); // also free tab??
		free(tmp);
		return (0);
	}
	return (add_key(key_val, data));
}
