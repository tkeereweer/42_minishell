/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:43:10 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/09 13:39:19 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//fd1 and fd2 are the duplicated stdin and out from run_builtins
//set to -1 if not in the context
void	ft_exit(char **argv, t_data *data, int fd1, int fd2)
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
	if (fd1 != -2 && fd2 != -2)
	{
		close(fd1);
		close(fd2);
		ft_printf("exit\n");
	}
	exit(n);
}
