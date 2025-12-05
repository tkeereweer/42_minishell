/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 10:20:16 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/05 10:48:55 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	run_builtins(char **argv, t_data *data, int mode)
{
	int	ret;

	ret = 0;
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		ft_echo(argv);
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		ret = ft_cd(argv[1], data);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		ret = ft_pwd();
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		ft_exit(argv, data);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		ft_env(data);
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		ret = ft_export(argv[1], data);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		ft_unset(&argv[1], data);
	if (mode != 4)
		exit(ret);
	return (ret);
}
