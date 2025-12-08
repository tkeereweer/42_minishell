/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 10:14:03 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/08 21:28:22 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int build_new_str(char **expanded, char **str, char *envvar, int env_pos)
{
	char	*new_str;

	if (!*expanded)
		*expanded = ft_strdup("");
	new_str = (char *) malloc((ft_strlen(*str) + ft_strlen_gnl(*expanded) - envvar_len(&(*str)[env_pos]) + 1) * sizeof(char));
	if (new_str == NULL)
		return (1);
	new_str[0] = '\0';
	ft_strncpy(new_str, *str, env_pos);
	ft_strcat(new_str, *expanded);
	ft_strcat(new_str, &(*str)[env_pos + envvar_len(&(*str)[env_pos])]);
	if (envvar[1] == '?' && ft_strlen(envvar) == 2)
		free(*expanded);
	free(envvar);
	free(*str);
	*str = new_str;
	return (0);
}

static int return_variable(char **expanded, char *envvar, t_data *data)
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

int	expand_envvars(char **str, t_data *data)
{
	int		i;

	i = 0;
	while ((*str)[i] != '\0')
	{
		if ((*str)[i] == '\'')
		{
			i++;
			while ((*str)[i] != '\'')
				i++;
		}
		else if ((*str)[i] == '"')
		{
			i++;
			if (expand_envvar_str(str, i, data, 0) == 1)
				return (-1);
			while ((*str)[i] != '"')
				i++;
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

int	expand_envvars_redir(char **str, t_data *data)
{
	int		i;

	i = 0;
	while ((*str)[i] != '\0')
	{
		if ((*str)[i] == '\'')
		{
			i++;
			while ((*str)[i] != '\'')
				i++;
		}
		else if ((*str)[i] == '"')
		{
			i++;
			if (expand_envvar_str(str, i, data, 0) == 1)
				return (-1);
			while ((*str)[i] != '"')
				i++;
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
