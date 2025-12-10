/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:40:54 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 12:13:26 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	copy_env(t_data *data, char **envp)
{
	int	i;

	i = 0;
	if (!*envp)
		return (set_minimal_env(data));
	while (envp[i] != NULL)
		i++;
	data->env = (char **) malloc((i + 2) * sizeof(char *));
	if (data->env == NULL)
		return (1);
	i = 0;
	while (envp[i] != NULL)
	{
		data->env[i] = ft_strdup(envp[i]);
		if (data->env[i] == NULL)
		{
			free_split(data->env);
			return (1);
		}
		i++;
	}
	data->env[i] = NULL;
	return (0);
}

t_data	init_data(char **envp)
{
	t_data	data;

	data.default_path = NULL;
	if (copy_env(&data, envp) == 1)
		exit(1);
	data.child_cnt = 0;
	data.cmd_cnt = 0;
	data.pid_tab = NULL;
	data.pipe_tab = NULL;
	data.line = NULL;
	data.prompt = NULL;
	data.exit_status = 0;
	return (data);
}

char	*build_prompt(t_data *data, char **envp)
{
	char	*user;
	char	buf[PATH_MAX];
	char	*prompt;

	if (!*envp)
		return (empty_env_prompt());
	user = ft_getenv("$USER", data->env);
	if (user == NULL)
	{
		user = ft_strdup("");
		if (!user)
			return (NULL);
	}
	if (getcwd(buf, PATH_MAX) == NULL)
		return (NULL);
	prompt = (char *) malloc((ft_strlen(user) + ft_strlen(buf) + 4)
			* sizeof(char));
	if (prompt == NULL)
		return (NULL);
	ft_strncpy(prompt, user, ft_strlen(user));
	ft_strcat(prompt, "@");
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
