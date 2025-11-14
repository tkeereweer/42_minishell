/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:12:30 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/16 12:37:37 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	i;

	src_len = ft_strlen(src);
	i = 0;
	if (dstsize == 0)
		return (src_len);
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	if (dstsize <= src_len)
		return (src_len);
	return (i);
}
/*
int main(void)
{
	char *test = "this is a test";
	char dest[49];
	size_t mylu = ft_strlcpy(dest, test, 10);
	char control[49];
	size_t theirlu = strlcpy(control, test, 10);
	printf("test: %s, stdcpy: %s, return: %lu\n mycpy: %s, return: %lu\n",
		 test, control, theirlu, dest, mylu);

}*/