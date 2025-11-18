/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 12:16:15 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/18 15:04:21 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern char	**environ;

int    export(char *key_val)
{
	int			i;

	i = 0;
	while (environ[i] != NULL)
		i++;
	environ = tab_realloc(environ, i + 1);
	if (environ == NULL)
		return (-1);
	environ[i] = key_val;
	return (0);
}

int	main(int argc, char *argv[])
{
	int	i;

	i = 0;
	if (argc < 2)
		return (1);
	export(argv[1]);
	while (environ[i] != NULL)
	{
		ft_printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}
