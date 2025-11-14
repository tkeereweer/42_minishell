/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 12:55:30 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/18 17:23:24 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 16
#endif

int	endof_line(char *str)
{
	if (!str)
		return (-1);
	while (*str)
	{
		if (*str == '\n')
			return (1);
		str++;
	}
	return (0);
}

size_t	ft_strlen_gnl(const char *str)
{
	size_t	n;

	n = 0;
	if (str == NULL)
		return (0);
	while (str[n])
		n++;
	return (n);
}

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t	n;

	n = 0;
	while (src[n] && n < len)
	{
		dst[n] = src[n];
		n++;
	}
	dst[n] = '\0';
	return (dst);
}

//null terminated if ptr !=NULL;
//l.62: fre(null) is a no-op in C so you dont have to check
char	*my_realloc(char *ptr, size_t size)
{
	char	*dst;
	size_t	ptr_len;

	if (!ptr)
		return ((char *) ft_calloc(size, 1));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	dst = (char *)malloc(size);
	if (!dst)
	{
		free(ptr);
		return (NULL);
	}
	ptr_len = ft_strlen_gnl(ptr);
	if (ptr_len + 1 < size)
		ft_strncpy(dst, ptr, ptr_len + 1);
	else
		ft_strncpy(dst, ptr, size);
	if (size > 0 && size <= ptr_len + 1)
		dst[size - 1] = '\0';
	free (ptr);
	return (dst);
}

char	*ft_strncat(char *dst, const char *src, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (dst[i])
		i++;
	j = 0;
	while (src[j] && j < n)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (dst);
}
