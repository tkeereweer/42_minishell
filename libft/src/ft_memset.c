/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:11:53 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/12 16:35:02 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t len)
{
	size_t			n;
	unsigned char	*ptr;

	n = 0;
	ptr = (unsigned char *) str;
	while (n < len)
	{
		ptr[n] = (unsigned char) c;
		n++;
	}
	return (str);
}

/*int main(void)
{
    char test[] = "";
    printf("%s\n", test);
    ft_memset(test, '.', 8);
    printf("%s\n", test);
    return (0);
}*/
