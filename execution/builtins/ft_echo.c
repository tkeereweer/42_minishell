/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:42:17 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/05 11:21:07 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	only_n(char *str)
{
	int	i;

	i = 1;
	while (str[i] != '\0')
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(char **args)
{
	int	i;
	int	mode;

	i = 1;
	mode = 0;
	while (args[i] != NULL && ft_strncmp(args[i], "-n", 2) == 0)
	{
		if (only_n(args[i]) == 0)
			break ;
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
