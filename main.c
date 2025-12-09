/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:07:57 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/09 14:41:58 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
