/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 10:17:54 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/18 17:03:40 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_in_set(char c, const char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *str, const char *set)
{
	size_t	len;
	size_t	i;
	size_t	j;

	if (!str)
		return (NULL);
	if (!set)
		return ((char *) str);
	i = 0;
	while (str[i] && is_in_set(str[i], set))
		i++;
	len = ft_strlen(str);
	j = 0;
	while (str[len - 1 - j] && is_in_set(str[len - 1 - j], set))
		j++;
	if (i >= len - j)
		return (ft_strdup(""));
	return (ft_substr(str, i, len - j - i));
}
/*
int main(void)
{
	char *test = "  \t \t \n   \n\n\n\t";
	char *set = " \n\t";
	char *dest = ft_strtrim(test, set);
	printf("str: %s, set: %s, result: %s", test, set, dest);
	free(dest);
}*/