/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 14:26:00 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/30 13:41:31 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

int	count_digits(int n)
{
	int	count;

	count = 0;
	if (n < 0)
	{
		n *= -1;
		count++;
	}
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		digits;
	int		i;
	char	*result;

	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	digits = count_digits(n);
	result = (char *) ft_calloc(digits + 1, sizeof(char));
	if (!result)
		return (NULL);
	i = 1;
	if (n < 0)
	{
		result[0] = '-';
		n *= -1;
	}
	while (n != 0)
	{
		result[digits - i++] = n % 10 + '0';
		n /= 10;
	}
	result[digits] = '\0';
	return (result);
}
/*
int main(void)
{
	int test = 9;
    char *dest = ft_itoa(test);
	printf("int: %d, digit count: %d, result: %s",
	 test, count_digits(test), dest);
    free(dest);
}*/