/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 10:14:03 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/08 17:00:30 by mturgeon         ###   ########.fr       */
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

int	expand_envvar_str(char **str, int i, t_data *data, int mode)
{
	int		env_pos;
	char    *itoa;
	char	*envvar;
	char	*expanded;
	char	*new_str;

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
		itoa = ft_itoa(data->exit_status);
		if (!itoa)
			return (free(envvar), 1);
		expanded = ft_strdup(itoa);
		free (itoa);
		if (expanded == NULL)
			return (free(envvar), 1);
	}
	else
		expanded = ft_getenv(envvar, data->env);
	if (!expanded)
		expanded = ft_strdup("");
	new_str = (char *) malloc((ft_strlen(*str) + ft_strlen_gnl(expanded) - envvar_len(&(*str)[env_pos]) + 1) * sizeof(char));
	if (new_str == NULL)
		return (1);
	new_str[0] = '\0';
	ft_strncpy(new_str, *str, env_pos);
	ft_strcat(new_str, expanded);
	ft_strcat(new_str, &(*str)[env_pos + envvar_len(&(*str)[env_pos])]);
	if (envvar[1] == '?' && ft_strlen(envvar) == 2)
		free(expanded);
	free(envvar);
	free(*str);
	*str = new_str;
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

static int is_ambiguous(char *str)
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
