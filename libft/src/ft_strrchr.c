/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:13:21 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/01 09:21:20 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	size_t	count;
	char	temp;

	i = 0;
	count = 0;
	temp = (char) c;
	while (s[i])
	{
		if (s[i] == temp)
			count = i;
		i++;
	}
	if (temp == '\0')
		return ((char *) &s[i]);
	if (count == 0 && s[0] != temp)
		return (NULL);
	return ((char *) &s[count]);
}
/*
int main(void)
{
	char *test = "this is a test";
	int c = '?';

	printf("haystack: %s\n, needle: %c\n, result: %s\n",
		 test, c, ft_strrchr(test, c));
}*/