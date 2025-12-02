/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:42:17 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/02 17:14:53 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_echo(char **args)
{
	int	i;
	int	mode;

	i = 1;
	mode = 0;
	while (args[i] != NULL && ft_strncmp(args[i], "-n", 3) == 0)
	{
		i++;
		mode = 1;
	}
	while (args[i] != NULL)
	{
		ft_printf("%s", args[i]);
		if (args[i + 1] != NULL)
			ft_printf("%c", ' ');
		i++;
	}
	if (mode != 1)
		ft_printf("%c", '\n');
}

// int	main(int argc, char *argv[])
// {
// 	if (argc < 2)
// 		return (1);
// 	echo(argv);
// 	perror("echo");
// 	return (0);
// }
