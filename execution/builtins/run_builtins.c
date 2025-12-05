/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 10:20:16 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/05 10:23:56 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//fd1 and fd2 are the duplicated stdin and out from run_builtins
//set to 0 if not in the context
int	run_builtins(char **argv, t_data *data, int fd1, int fd2)
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
		ft_exit(0, data, fd1, fd2);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		ft_env(data);
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		ret = ft_export(argv[1], data);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		ft_unset(&argv[1], data);
	return (ret);
}
