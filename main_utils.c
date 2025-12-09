/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 09:28:23 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 09:31:30 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void min_env_shlvl(t_data *data, char **temp, char *buf)
{
	ft_strcat(*temp, "PWD=");
	ft_strcat(*temp, buf);
	data->env[0] = *temp;
	data->env[1] = ft_strdup("SHLVL=1");
	return ;
}
int	set_minimal_env(t_data *data)
{
	char	buf[PATH_MAX];
	char	*temp;
	ssize_t	len;

	if (!getcwd(buf, PATH_MAX))
		return (1);
	data->env = (char **)malloc(4 * sizeof(char *));
	if (!data->env)
		return (1);
	len = ft_strlen(buf) + ft_strlen("PWD=") + 1;
	temp = (char *)malloc(len);
	if (!temp)
		return (free(data->env), 1);
	min_env_shlvl(data, &temp, buf);
	if (!data->env[1])
		return (free_split(data->env), 1);
	data->env[2] = ft_strdup("_=/usr/bin/env");
	if (!data->env[2])
		return (free_split(data->env), 1);
	data->env[3] = NULL;
	data->default_path = ft_strdup("/usr/local/bin:/usr/local/sbin:/usr/sbin:/usr/bin:/sbin:/bin:.");
	if (data->default_path == NULL)
		return (free_split(data->env), 1);
	return (0);
}

char	*empty_env_prompt()
{
	char    *prompt;
	char	buf[PATH_MAX];

	getcwd(buf, PATH_MAX);
	prompt = (char *)malloc(ft_strlen("empty_env:") + ft_strlen(buf) + 3);
	if (!prompt)
		return (NULL);
	ft_strncpy(prompt, "empty_env:", ft_strlen("empty_env:"));
	ft_strcat(prompt, buf);
	ft_strcat(prompt, ": ");
	return (prompt);
}

void	clean_exit(t_data *data, char *line, char *prompt)
{
	if (line != NULL)
		free(line);
	if (prompt != NULL)
		free(prompt);
	free_split(data->env);
	if (data->default_path != NULL)
		free(data->default_path);
	rl_clear_history();
	exit(1);
}

int	only_whitespace(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (ft_is_whitespace(line[i]) != 1)
			return (0);
		i++;
	}
	return (1);
}