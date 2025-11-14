/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printfutils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 08:35:35 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/14 11:48:01 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	count_digits(long n)
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

//flags: space adds ' ' if n > 0. plus adds '+' if n > 0
char	*ft_ltoa(long n)
{
	int		digits;
	int		i;
	char	*result;

	if (n == 0)
		return (ft_strdup("0"));
	digits = count_digits(n);
	result = (char *) ft_calloc(digits + 1, 1);
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

static char	*ft_utoa(unsigned int n)
{
	int		digits;
	int		i;
	char	*result;

	if (n == 0)
		return (ft_strdup("0"));
	digits = count_digits(n);
	result = (char *) ft_calloc(digits + 1, sizeof(char));
	if (!result)
		return (NULL);
	i = 1;
	while (n != 0)
	{
		result[digits - i++] = n % 10 + '0';
		n /= 10;
	}
	result[digits] = '\0';
	return (result);
}

size_t	ft_putnbr_u(unsigned int n)
{
	char	*result;
	size_t	len;

	result = ft_utoa(n);
	len = ft_strlen(result);
	ft_putstr(result);
	free(result);
	return (len);
}
