/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:12:04 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/01 11:16:40 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <string.h>
//#include <stdio.h>

//first occurence of c in s
char	*ft_strchr(const char *s, int c)
{
	char	temp;

	temp = (char) c;
	while (*s)
	{
		if (*s == temp)
			return ((char *) s);
		s++;
	}
	if (temp == '\0')
		return ((char *) s);
	return (NULL);
}
/*
int main(void)
{
	char *s = "test";
	int c = '\0';
	printf("theirs:%s\n, mine :%s\n", strchr(s, c), ft_strchr(s, c));
}*/