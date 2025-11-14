/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:11:48 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/15 17:02:34 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			n;
	unsigned char	*ptr_1;
	unsigned char	*ptr_2;

	if (!dst && !src)
		return (NULL);
	n = -1;
	ptr_1 = (unsigned char *) src;
	ptr_2 = (unsigned char *) dst;
	if (src <= dst)
	{
		while (++n < len)
			ptr_2[len - 1 - n] = ptr_1[len - 1 - n];
	}
	else
	{
		while (++n < len)
			ptr_2[n] = ptr_1[n];
	}
	return (dst);
}

/*
int main(void)
{
	int src[100] = {1, 2, 3, 4, 5, 6, 7};
	int dest[100];
	 int i = -1;

	printf("src:\n");
	 while (++i < 6)
	     printf("src[%d]: %d\n", i, src[i]);
	ft_memmove(dest, src, 5 * sizeof(int));
	printf("dest:\n");
    i = -1;
    while (++i < 6)
        printf("dest[%d]: %d\n", i, dest[i]);

    char *src = (void * ) 0;
    char dest[100];
    printf ("src:%s\n", src);
    ft_memmove(dest, src, 5);
    printf("dest:%s\n", dest);

	int src = 60;
	int dest;

	printf("src: %d\n", src);
	ft_memmove(&dest, &src, sizeof(int));
	printf("dest: %d\n", dest);
}*/