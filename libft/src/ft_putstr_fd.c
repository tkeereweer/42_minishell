/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:40:21 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/18 21:26:09 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *str, int fd)
{
	if (!str)
		return ;
	write(fd, str, ft_strlen(str));
	return ;
}

/*
int main(void)
{
    int fd = open("string", O_CREAT | O_RDWR, 0644);
    ft_putendl_fd("this is a test", fd);
    close(fd);
    return (0);
}*/