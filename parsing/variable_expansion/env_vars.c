/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 10:14:03 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 11:50:26 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	build_new_str(char **expand, char **str, char *envvar, int env_pos)
{
	char	*new_str;

	if (!*expand)
		*expand = ft_strdup("");
	new_str = (char *) malloc((ft_strlen(*str) + ft_strlen_gnl(*expand)
				- envvar_len(&(*str)[env_pos]) + 1) * sizeof(char));
	if (new_str == NULL)
		return (1);
	new_str[0] = '\0';
	ft_strncpy(new_str, *str, env_pos);
	ft_strcat(new_str, *expand);
	ft_strcat(new_str, &(*str)[env_pos + envvar_len(&(*str)[env_pos])]);
	if (envvar[1] == '?' && ft_strlen(envvar) == 2)
		free(*expand);
	free(envvar);
	free(*str);
	*str = new_str;
	return (0);
}

static int	return_variable(char **expanded, char *envvar, t_data *data)
{
	char	*itoa;

	itoa = ft_itoa(data->exit_status);
	if (!itoa)
		return (free(envvar), 1);
	*expanded = ft_strdup(itoa);
	free (itoa);
	if (*expanded == NULL)
		return (free(envvar), 1);
	return (0);
}

int	expand_envvar_str(char **str, int i, t_data *data, int mode)
{
	int		env_pos;
	char	*envvar;
	char	*expanded;

	env_pos = has_envvar(*str, i, mode);
	if (env_pos == -1)
		return (0);
	envvar = ft_substr(*str, env_pos, envvar_len(&(*str)[env_pos]));
	if (envvar == NULL)
		return (1);
	if (envvar[0] == '$' && ft_strlen(envvar) == 1)
		return (free(envvar), 0);
	if (envvar[1] == '?' && ft_strlen(envvar) == 2)
	{
		if (return_variable(&expanded, envvar, data))
			return (1);
	}
	else
		expanded = ft_getenv(envvar, data->env);
	if (build_new_str(&expanded, str, envvar, env_pos))
		return (1);
	return (expand_envvar_str(str, i, data, mode));
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

int	expand_envvars(char **str, t_data *data)
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
			if ((*str)[0] != '\0' && envvar_len(&(*str)[i]) == 1)
				i++;
			continue ;
		}
		i++;
	}
	return (0);
}
