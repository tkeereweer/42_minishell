/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:12:23 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/25 13:44:07 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

//dst_len is calculated where it is to allow a no crash return when dstsize = 0
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;
	size_t	j;

	src_len = ft_strlen(src);
	if (dstsize == 0)
		return (src_len);
	dst_len = ft_strlen(dst);
	if (dstsize <= dst_len)
		return (dstsize + src_len);
	i = 0;
	j = 0;
	while (dst[i])
		i++;
	while (src[j] && i < dstsize - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (dst_len + src_len);
}
/*
int main(void)
{
	char dest[100] = {0};
	//char *dest_control;
	//dest_control = NULL;
	ft_memset(dest, 'r', 15);
	printf("memset success\n");
	printf("strlen dest: %lu\n", ft_strlen(dest));
	//char control[100] = { 'a', ' ', 'b'};
	char *test = "lorem ipsum"; //15 characters
	
	ft_strlcat(dest, test, 20);
	//strlcat(dest_control, test, 5);

	printf("mine: %s\n", dest);

}*/