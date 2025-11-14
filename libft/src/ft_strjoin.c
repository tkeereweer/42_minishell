/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 18:00:35 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/15 18:25:41 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*dst;

	if (!s1 || !s2)
		return (NULL);
	dst = (char *) malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!dst)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
		dst[i++] = s1[j++];
	j = 0;
	while (s2[j])
		dst[i++] = s2[j++];
	dst[i] = '\0';
	return (dst);
}

/*int main(void)
{
	char *s1 = "my favorite animal is ";
	char *s2 = "a bird";
	char *result = ft_strjoin(s2, s1);

	printf("len_s1 = %lu, len_s2 = %lu, s1: %s, s2: %s\n, result: %s\n",
	ft_strlen(s1), ft_strlen(s2), s1, s2, result);

    char *s1 = "my favorite animal is ";
    char *s2 = s1 + 20;
    printf("s1: %p, s2: %p\n, *s1: %c, *s2: %c", s1, s2, *s1, *s2);
}*/