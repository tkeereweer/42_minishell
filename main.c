/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:07:57 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/25 09:34:47 by mkeerewe         ###   ########.fr       */
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

int	run_line(char *line, char **path_tab)
{
	t_list	*list;
	t_node	*tree;

	if (line != NULL && ft_strlen_gnl(line) != 0) // what if only spaces
	{
		add_history(line);
		list = clean_node_list(line, path_tab);
		if (list != NULL)
		{
			tree = create_logic_tree(list);
			if (create_cmd_trees(tree) == 1)
			{
				free_tree(tree);
				return (1);
			}
			// draw_tree(tree);
			print_tree(tree);
			free_tree(tree);
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
	prompt = (char *) malloc((ft_strlen(user) + ft_strlen(buf) + 1) * sizeof(char));
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
	char	**tab;
	char	*prompt;

	(void) argc;
	(void) argv;
	if (handle_signals_parent() == 1)
		return (1);
	// tab = malloc(5 *sizeof(char *));
	// tab[0] = ft_strdup("cmd");
	// tab[1] = ft_strdup("t***.**");
	// tab[2] = ft_strdup("last");
	// tab[3] = ft_strdup("\"$?\"");
	// tab[4] = NULL;
	if (copy_env(&data, envp) == 1)
		return (1);
	data.path_tab = NULL;
	// ft_export("?=1", &data);
	// ft_cd("parsing", &data);
	// ft_pwd();
	// ft_env(&data);
	// expand_vars(&tab, &data);
	prompt = build_prompt(&data);
	if (prompt == NULL)
		clean_exit(&data, NULL);
	line = readline(prompt);
	free(prompt);
	if (run_line(line, data.path_tab) == 1)
		clean_exit(&data, line);
	while (line != NULL)
	{
		free(line);
		prompt = build_prompt(&data);
		if (prompt == NULL)
			clean_exit(&data, NULL);
		line = readline(prompt);
		free(prompt);
		if (run_line(line, data.path_tab) == 1)
			clean_exit(&data, line);
	}
	ft_printf("exit\n");
	free_split(data.env);
	free_split(tab);
	rl_clear_history();
	return (0);
}
