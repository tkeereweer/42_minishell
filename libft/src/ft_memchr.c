/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:11:30 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/15 16:26:08 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *) str;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == (unsigned char) c)
			return (&ptr[i]);
		i++;
	}
	return (NULL);
}

/*
int main(void)
{
    char *test = "abcdefg";
    int needle = 'd';
    printf("needle: %d, haystack: %s, result: %s\n", 
	needle, test, (char *)ft_memchr(test, needle, 7));
}*/