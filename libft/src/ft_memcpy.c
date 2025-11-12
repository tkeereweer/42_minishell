/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:11:40 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/15 15:13:23 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*ptr_1;
	unsigned char	*ptr_2;

	if (dst == NULL && src == NULL)
		return (NULL);
	ptr_1 = (unsigned char *) src;
	ptr_2 = (unsigned char *) dst;
	i = 0;
	while (i < n)
	{
		ptr_2[i] = ptr_1[i];
		i++;
	}
	return (dst);
}

/*int main(void)
{
	char *dst = "test";
	char *src = "ohno";
	ft_memcpy(dst, src, NULL);
}*/