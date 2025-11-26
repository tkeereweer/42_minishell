/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 10:20:16 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/26 10:33:19 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	run_builtins(char **argv, t_data *data)
{
	int	ret;

	ret = 0;
	if (strcmp(argv[0], "echo") == 0)
		ft_echo(argv);
	else if (strcmp(argv[0], "cd") == 0)
		ret = ft_cd(argv[1], data);
	else if (strcmp(argv[0], "pwd") == 0)
		ret = ft_pwd();
	else if (strcmp(argv[0], "exit") == 0)
		ft_exit(0);
	else if (strcmp(argv[0], "env") == 0)
		ft_env(data);
	else if (strcmp(argv[0], "export") == 0)
		ret = ft_export(argv[1], data);
	else if (strcmp(argv[0], "unset") == 0)
		ft_unset(argv[1], data);
	return (ret);
}
