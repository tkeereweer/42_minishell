/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 12:37:46 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/15 13:37:02 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//numbers after O_RDWR are permission values. 0644 is safe.
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

/*int main(void)
{
	int fd =  open("z", O_CREAT | O_RDWR, 0644);
	ft_putchar_fd('z', fd);
	close(fd);
	return (0);
}*/