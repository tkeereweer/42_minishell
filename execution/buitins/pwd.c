/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 12:07:54 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/18 12:13:49 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int pwd(void)
{
	char buf[PATH_MAX];

	if (getcwd(buf, PATH_MAX) == NULL)
		return (-1);
	ft_printf("%s", buf);
	return (0);
}

int	main(void)
{
	pwd();
	return (0);
}
