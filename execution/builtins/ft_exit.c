/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:43:10 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/03 14:35:26 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_exit(unsigned int n, t_data *data)
{
	free_split(data->env);
	free(data->pid_tab);
	free_tree(data->tree);
	rl_clear_history();
	ft_printf("exit\n");
	exit(n);
}
