/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:07:57 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/02 09:23:59 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tree(t_node *node)
{
	int	i;

	i = 0;
	if (node == NULL)
		return ;
	print_tree(node->left_child);
	if (node->type == PIPELINE)
		ft_printf("%s\n", "PIPELINE");
	else if (node->type == CMD)
		ft_printf("%s\n", "CMD");
	else if (node->type == LOGIC)
	{
		if (node->content.logic == AND)
			ft_printf("%s\n", "AND");
		else if (node->content.logic == OR)
			ft_printf("%s\n", "OR");
	}
	else if (node->type == ARGS)
	{
		ft_printf("%s: ", "ARGS");
		while (node->content.tab[i] != NULL)
		{
			ft_printf("%s; ", node->content.tab[i]);
			i++;
		}
		ft_printf("%c", '\n');
	}
	else if (node->type == REDIR)
	{
		ft_printf("%s: ", "REDIR");
		ft_printf("%s; ", "**kind**");
		ft_printf("%s;\n", node->content.redir.path);
	}
	print_tree(node->right_child);
}

int	copy_env(t_data *data, char **envp)
{
	int	i;

	i = 0;
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

void	clean_exit(t_data *data, char *line)
{
	if (line != NULL)
		free(line);
	free_split(data->env);
	rl_clear_history();
	exit(1);
}

int	run_line(char *line, t_data *data)
{
	t_list	*list;
    char    **temp;
    int     i;
    temp = NULL;
	if (line != NULL && ft_strlen_gnl(line) != 0) // what if only spaces
	{
		add_history(line);
        i = 0;
        while (ft_is_whitespace(line[i]))
            i++;
        if (!line[i])
            return (0);
		list = clean_node_list(line, &temp);
		if (list != NULL)
		{
			data->tree = create_logic_tree(list);
			if (create_cmd_trees(data->tree) == 1)
			{
				free_tree(data->tree);
                clean_path_tab(temp);
				return (1);
			}
			exec_tree(data->tree, data);
			free_tree(data->tree);
            clean_path_tab(temp);
		}
	}
	return (0);
}

char	*build_prompt(t_data *data)
{
	char	*user;
	char	buf[PATH_MAX];
	char	*prompt;

	user = ft_getenv("$USER", data->env);
	if (user == NULL)
		return (NULL);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (NULL);
	prompt = (char *) malloc((ft_strlen(user) + ft_strlen(buf) + 4) * sizeof(char));
	if (prompt == NULL)
		return (NULL);
	ft_strncpy(prompt, user, ft_strlen(user));
	ft_strcat(prompt, "@");
	ft_strcat(prompt, buf);
	ft_strcat(prompt, ": ");
	return (prompt);
}

int	main(int argc, char *argv[], char **envp)
{
	char	*line;
	t_data	data;
	char	*prompt;

	(void) argc;
	(void) argv;
	if (handle_signals_parent() == 1)
		return (1);
	if (copy_env(&data, envp) == 1)
		return (1);
	data.child_cnt = 0;
	data.cmd_cnt = 0;
	data.pid_tab = NULL;
	data.pipe_tab = NULL;
	data.exit_status = 0;
	prompt = build_prompt(&data);
	if (prompt == NULL)
		clean_exit(&data, NULL);
	line = readline(prompt);
	free(prompt);
	if (run_line(line, &data) == 1)
		clean_exit(&data, line);
	while (line != NULL)
	{
		free(line);
		prompt = build_prompt(&data);
		if (prompt == NULL)
			clean_exit(&data, NULL);
		line = readline(prompt);
		free(prompt);
		if (run_line(line, &data) == 1)
			clean_exit(&data, line);
	}
	ft_printf("exit\n");
	free_split(data.env);
	rl_clear_history();
	return (0);
}
