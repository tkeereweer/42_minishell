/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 10:14:03 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/01 14:43:45 by mkeerewe         ###   ########.fr       */
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

int	has_envvar(char *str, int i)
{
	while (str[i] != '\0' && str[i] != '"')
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

char	*ft_strcat(char *dst, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dst[i] != '\0')
		i++;
	while (src[j] != '\0')
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (dst);
}

int	expand_envvar_str(char **str, int i, t_data *data)
{
	int		env_pos;
	char	*envvar;
	char	*expanded;
	char	*new_str;

	env_pos = has_envvar(*str, i);
	if (env_pos == -1)
		return (0);
	envvar = ft_substr(*str, env_pos, envvar_len(&(*str)[env_pos]));
	if (envvar == NULL)
		return (1);
	if (envvar[0] == '$' && ft_strlen(envvar) == 1)
	{
		free(envvar);
		return (0);
	}
	if (envvar[1] == '?' && ft_strlen(envvar) == 2)
	{
		expanded = ft_strdup(ft_itoa(data->exit_status));
		if (expanded == NULL)
		{
			free(envvar);
			return (1);
		}
	}
	else
		expanded = ft_getenv(envvar, data->env);
	new_str = (char *) malloc((ft_strlen(*str) + ft_strlen_gnl(expanded) - envvar_len(&(*str)[env_pos]) + 1) * sizeof(char));
	if (new_str == NULL)
		return (1);
	new_str[0] = '\0';
	ft_strlcpy(new_str, *str, env_pos);
	if (expanded != NULL)
		ft_strcat(new_str, expanded);
	ft_strcat(new_str, &(*str)[env_pos + envvar_len(&(*str)[env_pos])]);
	if (envvar[1] == '?' && ft_strlen(envvar) == 2)
		free(expanded);
	free(envvar);
	free(*str);
	*str = new_str;
	return (expand_envvar_str(str, i, data));
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
			if (expand_envvar_str(str, i, data) == 1)
				return (-1);
			while ((*str)[i] != '"')
				i++;
		}
		else if ((*str)[i] == '$')
		{
			if (expand_envvar_str(str, i, data) == 1)
				return (-1);
			if ((*str)[0] != '\0' && envvar_len(&(*str)[i]) == 1)
				i++;
			continue ;
		}
		i++;
	}
	return (0);
}

// int	main(int argc, char *argv[], char *envp[])
// {
// 	char *str;
// 	t_data data;

// 	if (argc < 0)
// 		return (0);
// 	str = malloc(20);
// 	str[0] = '\0';
// 	ft_strcat(str, "$USER\"$SHELL\"");
// 	data.env = envp;
// 	new_expand_envvars(&str, &data);
// 	ft_printf(str);
// 	free(str);
// 	return (0);
// }

