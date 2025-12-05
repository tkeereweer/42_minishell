/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:43:10 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/05 10:48:30 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//fd1 and fd2 are the duplicated stdin and out from run_builtins
//set to -1 if not in the context
void	ft_exit(unsigned int n, t_data *data, int fd1, int fd2)
{
	free_split(data->env);
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
