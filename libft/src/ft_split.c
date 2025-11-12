/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 11:06:27 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/19 21:03:20 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

size_t	string_count(const char *str, char c)
{
	size_t	i;
	size_t	j;
	size_t	count;

	i = 0;
	j = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		j = i;
		while (str[i] != c && str[i])
			i++;
		if (j < i)
			count++;
	}
	return (count);
}

static void	clean_free(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**splittin_stuff(const char *str, char c, char **tab)
{
	size_t	i;
	size_t	j;
	size_t	n;

	i = 0;
	j = 0;
	n = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		j = i;
		while (str[i] != c && str[i])
			i++;
		if (j < i)
		{
			tab[n] = ft_substr(str, j, i - j);
			if (!tab[n])
				return (clean_free(tab), NULL);
			n++;
		}
	}
	tab[n] = NULL;
	return (tab);
}

char	**ft_split(const char *str, char c)
{
	size_t	count;
	char	**result;

	if (!str)
		return (NULL);
	count = string_count(str, c) + 1;
	result = (char **) ft_calloc(count, sizeof(char *));
	if (!result)
		return (NULL);
	return (splittin_stuff(str, c, result));
}
/*
int main(void)
{
	char *test = "          ";
	char sep = ' ';
	size_t  n = 0;

	char **result = ft_split(test, sep);
	printf("string: %s, sep: %c, result:\n", test, sep);
	while(result[n])
	{
		printf("(%lu): [%s]\n", n, result[n]);
		n++;
	}
	n = 0;
	while (result[n])
	{
		free(result[n]);
		n++;
	}
	free(result);
	result = NULL; //prevents accidental use of freed memory
}*/