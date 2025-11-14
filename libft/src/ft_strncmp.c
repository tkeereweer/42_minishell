/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:12:58 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/30 12:06:55 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	i = 0;
	if (n == 0)
		return (0);
	while (*str1 && *str2 && i < n - 1 && *str1 == *str2)
	{
		str1++;
		str2++;
		i++;
	}
	return (*str1 - *str2);
}
/*
int main(void)
{
	char *s2 = "abcdef";
	char *s1 = "abcdefghijk";

	printf("s1: %s, s2: %s, my result: %d, stdlib result: %d\n",
			s1, s2, ft_strncmp(s1, s2, 0), strncmp(s1, s2, 0));
}*/
