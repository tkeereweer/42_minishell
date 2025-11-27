/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 10:14:03 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/27 18:37:03 by mkeerewe         ###   ########.fr       */
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

int	has_envvar(char *str)
{
	int	i;

	i = 0;
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
		return (i);
	while (ft_isalnum(str[i]) == 1 || str[i] == '_')
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

int	expand_envvars(char **str, t_data *data)
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

int	new_expand_envvars(char **str, t_data *data)
{
	int	i;

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
			expand_envvars(str, data);
			while ((*str)[i] != '"')
				i++;
		}
		i++;
	}
	new_remove_quotes(str, '"');
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	char *str;
	t_data data;

	if (argc < 0)
		return (0);
	str = malloc(20);
	str[0] = '\0';
	ft_strcat(str, "\"$USER\"\"$SHELL\"");
	data.env = envp;
	new_expand_envvars(&str, &data);
	free(str);
	return (0);
}

