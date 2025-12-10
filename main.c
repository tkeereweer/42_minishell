/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:07:57 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 12:12:29 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signum = 0;

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
			return (0);
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

void	handle_next_cmd(t_data *data, char **envp)
{
	char	**temp;

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

int	main(int argc, char *argv[], char **envp)
{
	t_data	data;

	(void) argc;
	(void) argv;
	data = init_data(envp);
	handle_next_cmd(&data, envp);
	while (data.line != NULL)
	{
		free(data.line);
		handle_next_cmd(&data, envp);
	}
	ft_printf("exit\n");
	free_split(data.env);
	if (data.default_path != NULL)
		free(data.default_path);
	rl_clear_history();
	return (0);
}
