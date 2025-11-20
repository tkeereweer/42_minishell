/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 10:14:03 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/19 14:57:53 by mkeerewe         ###   ########.fr       */
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

int has_envvar(char *str)
{
	int	i;
	int	idx;

	i = 0;
	idx = -1;
	while (str[i] != '\0' && str[i + 1] != '\0')
	{
		if (str[i] == ' ' && str[i + 1] == '$')
		{
			idx = i + 1;
			break ;
		}
		if (i == 0 && str[i] == '$')
		{
			idx = i;
			break ;
		}
		i++;
	}
	if (idx > -1 && str[idx + 1] != ' ' && str[idx + 1] != '\0')
		return (idx);
	return (-1);
}

int envvar_len(char *str)
{
	int	i;

	i = 1;
	while (str[i] != '\0' && str[i] != ' ' && str[i] != '$')
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

int expand_envvars(char **str, t_data *data)
{
	int		env_pos;
	char	*envvar;
	char	*expanded;
	char	*new_str;

	env_pos = has_envvar(*str);
	if (env_pos == -1)
		return (0);
	envvar = ft_substr(*str, env_pos, envvar_len(&(*str)[env_pos]));
	if (envvar == NULL)
		return (1);
	expanded = ft_getenv(envvar, data->env);
	free(envvar);
	new_str = (char *) malloc((ft_strlen(*str) + ft_strlen_gnl(expanded) - envvar_len(&(*str)[env_pos]) + 1) * sizeof(char));
	if (new_str == NULL)
		return (1);
	new_str[0] = '\0';
	ft_strncpy(new_str, *str, env_pos);
	if (expanded != NULL)
		ft_strcat(new_str, expanded);
	ft_strcat(new_str, &(*str)[env_pos + envvar_len(&(*str)[env_pos])]);
	free(*str);
	*str = new_str;
	return (expand_envvars(str, data));
}
