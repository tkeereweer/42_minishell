/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 12:07:54 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/26 14:12:07 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_pwd(void)
{
	char	buf[PATH_MAX];

	if (getcwd(buf, PATH_MAX) == NULL)
		return (-1);
	ft_printf("%s\n", buf);
	return (0);
}

// #ifndef TESTING

// int	main(void)
// {
// 	ft_pwd();
// 	return (0);
// }
// #endif
