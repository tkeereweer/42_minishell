/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:13:03 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/01 11:17:29 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

//len is max number of char searched
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	n_len;

	i = 0;
	n_len = ft_strlen(needle);
	if (n_len == 0)
		return ((char *) haystack);
	while (i < len && haystack[i] && n_len <= len - i)
	{
		if (haystack[i] == needle[0])
			if (!(ft_strncmp(&haystack[i], needle, n_len)))
				return ((char *) &haystack[i]);
		i++;
	}
	return (NULL);
}
/*
int main(void)
{
	char *hay = "aaabcabcd";
	char *needle = "aabc";

	printf("result:%s\n", ft_strnstr(hay, needle, -1));
}*/