/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:07:57 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/04 13:28:54 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signum = 0;

// void	print_tree(t_node *node)
// {
// 	int	i;

// 	i = 0;
// 	if (node == NULL)
// 		return ;
// 	print_tree(node->left_child);
// 	if (node->type == PIPELINE)
// 		ft_printf("%s\n", "PIPELINE");
// 	else if (node->type == CMD)
// 		ft_printf("%s\n", "CMD");
// 	else if (node->type == LOGIC)
// 	{
// 		if (node->content.logic == AND)
// 			ft_printf("%s\n", "AND");
// 		else if (node->content.logic == OR)
// 			ft_printf("%s\n", "OR");
// 	}
// 	else if (node->type == ARGS)
// 	{
// 		ft_printf("%s: ", "ARGS");
// 		while (node->content.tab[i] != NULL)
// 		{
// 			ft_printf("%s; ", node->content.tab[i]);
// 			i++;
// 		}
// 		ft_printf("%c", '\n');
// 	}
// 	else if (node->type == REDIR)
// 	{
// 		ft_printf("%s: ", "REDIR");
// 		ft_printf("%s; ", "**kind**");
// 		ft_printf("%s;\n", node->content.redir.path);
// 	}
// 	print_tree(node->right_child);
// }

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
	ft_strcat(temp, "PWD=");
	ft_strcat(temp, buf);
	data->env[0] = temp;
	data->env[1] = ft_strdup("SHLVL=1");
	if (!data->env[1])
		return (free_split(data->env), 1);
	data->env[2] = ft_strdup("_=/usr/bin/env");
	if (!data->env[2])
		return (free_split(data->env), 1);
	data->env[3] = NULL;
	return (0);
}

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

void	clean_exit(t_data *data, char *line, char *prompt)
{
	if (line != NULL)
		free(line);
	if (prompt != NULL)
		free(prompt);
	free_split(data->env);
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

int	run_line(char *line, t_data *data)
{
	t_list	*list;
	char	**temp;
	int		res;

	temp = NULL;
	if (line != NULL && ft_strlen_gnl(line) != 0)
	{
		add_history(line);
		if (only_whitespace(line) == 1)
			return (0);
		list = clean_node_list(line, &temp);
		if (list == NULL)
			return (1);
		data->tree = create_logic_tree(list);
		res = create_cmd_trees(data->tree);
		if (res == 1)
		{
			free_tree(data->tree);
			clean_path_tab(temp);
			return (1);
		}
		if (res == 2)
			return (0);
		if (g_signum == 0)
			exec_tree(data->tree, data);
		else
			data->exit_status = 128 + g_signum;
		free_tree(data->tree);
		if (clean_path_tab(temp) == -1)
			return (1);
	}
	return (0);
}

t_data	init_data(char **envp)
{
	t_data	data;

	if (copy_env(&data, envp) == 1)
		exit(1);
	data.default_path = ft_strdup("/usr/local/bin:/usr/local/sbin:/usr/sbin:/usr/bin:/sbin:/bin:.");
	data.child_cnt = 0;
	data.cmd_cnt = 0;
	data.pid_tab = NULL;
	data.pipe_tab = NULL;
	data.exit_status = 0;
	return (data);
}

char	*build_prompt(t_data *data, char **envp)
{
	char	*user;
	char	buf[PATH_MAX];
	char	*prompt;

	if (!*envp)
	{
		getcwd(buf, PATH_MAX);
		prompt = (char *)malloc(ft_strlen("empty_env:") + ft_strlen(buf) + 3);
		if (!prompt)
			return (NULL);
		ft_strncpy(prompt, "empty_env:", ft_strlen("empty_env:"));
		ft_strcat(prompt, buf);
		ft_strcat(prompt, ": ");
		return (prompt);
	}
	user = ft_getenv("$USER", data->env);
	if (user == NULL)
		return (NULL);
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

void	handle_next_cmd(t_data *data, char **line, char **envp)
{
	char	*prompt;

	prompt = build_prompt(data, envp);
	if (prompt == NULL)
		clean_exit(data, NULL, NULL);
	g_signum = 0;
	if (handle_signals_parent(0) == 1)
		clean_exit(data, *line, prompt);
	*line = readline(prompt);
	free(prompt);
	if (handle_signals_parent(1) == 1)
		clean_exit(data, *line, NULL);
	if (run_line(*line, data) == 1)
		clean_exit(data, *line, NULL);
}

int	main(int argc, char *argv[], char **envp)
{
	t_data	data;
	char	*line;

	(void) argc;
	(void) argv;
	line = NULL;
	data = init_data(envp);
	handle_next_cmd(&data, &line, envp);
	while (line != NULL)
	{
		free(line);
		handle_next_cmd(&data, &line, envp);
	}
	ft_printf("exit\n");
	free_split(data.env);
	free(data.default_path);
	rl_clear_history();
	return (0);
}
