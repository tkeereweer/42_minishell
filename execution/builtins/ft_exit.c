/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:43:10 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/05 10:48:42 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_exit(char **argv, t_data *data)
{
	unsigned int	n;

	n = 0;
	if (argv[1] != NULL)
		n = ft_atoi(argv[1]);
	free_split(data->env);
	if (data->default_path != NULL)
		free(data->default_path);
	free(data->pid_tab);
	free_tree(data->tree);
	rl_clear_history();
	ft_printf("exit\n");
	exit(n);
}
