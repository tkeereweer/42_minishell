/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 10:17:54 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 12:09:20 by mturgeon         ###   ########.fr       */
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
	while (j < len - 1 && is_in_set(str[len - 1 - j], set))
		j++;
	if (i >= len - j)
		return (ft_strdup(""));
	return (ft_substr(str, i, len - j - i));
}
