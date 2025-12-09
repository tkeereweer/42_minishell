/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:40:54 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 14:50:01 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signum = 0;

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

//temp is initialized to NULL outside of here to save two lines
int	run_line(char **line, t_data *data, char **temp)
{
	t_list	*list;
	int		res;

	if (*line != NULL && ft_strlen_gnl(*line) != 0)
	{
		add_history(*line);
		if (only_whitespace(*line) == 1)
			return (0);
		list = clean_node_list(line, &temp, data);
		if (list == NULL)
			return (0);// shouldn't we return 1 ??
		data->tree = create_logic_tree(list);
		res = create_cmd_trees(data->tree);
		if (res == 1)
			return (free_tree(data->tree), clean_path_tab(temp), 1);
		if (res == 2)
			return (0);
		exec_tree(data->tree, data);
		free_tree(data->tree);
		clean_path_tab(temp);
	}
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

void	handle_next_cmd(t_data *data, char **envp)
{
    char    **temp;

    temp = NULL;
	data->prompt = build_prompt(data, envp);
	if (data->prompt == NULL)
		clean_exit(data, NULL, NULL);
	g_signum = 0;
	if (handle_signals_parent(0) == 1)
		clean_exit(data, data->line, data->prompt);
	data->line = readline(data->prompt);
	if (g_signum != 0)
		data->exit_status = 128 + g_signum;
	free(data->prompt);
	if (handle_signals_parent(1) == 1)
		clean_exit(data, data->line, NULL);
	if (run_line(&(data->line), data, temp) == 1)
		clean_exit(data, data->line, NULL);
}