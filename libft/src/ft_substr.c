/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:06:29 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/01 11:03:34 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dst;
	size_t	i;

	if (!s)
		return (NULL);
	dst = (char *) malloc((len + 1) * sizeof(char));
	if (!dst)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		dst[0] = '\0';
		return (dst);
	}
	i = 0;
	while (s[start + i] && i < len)
	{
		dst[i] = s[start + i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
/*
int main(void)
{
    char *test = "this is my test string it must be a bit long";
    char *result = ft_substr(test, 100, 1);

    printf("original:%s, sub:%s\n", test, result);
    free(result);
}*/