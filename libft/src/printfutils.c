/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printfutils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 15:48:13 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/26 17:41:37 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	count_digits_hex(unsigned int n)
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
		n /= 16;
		count++;
	}
	return (count);
}

static int	count_digits_hex_long(unsigned long n)
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
		n /= 16;
		count++;
	}
	return (count);
}

char	*ft_ltohex(unsigned int n)
{
	int		digits;
	int		i;
	char	*result;
	char	*base;

	base = "0123456789abcdef";
	if (n == 0)
		return (ft_strdup("0"));
	digits = count_digits_hex(n);
	result = (char *) ft_calloc(digits + 1, sizeof(char));
	if (!result)
		return (NULL);
	i = 1;
	while (n != 0)
	{
		result[digits - i++] = base[n % 16];
		n /= 16;
	}
	result[digits] = '\0';
	return (result);
}

char	*ft_ltohex_long(unsigned long n)
{
	int		digits;
	int		i;
	char	*result;
	char	*base;

	base = "0123456789abcdef";
	if (n == 0)
		return (ft_strdup("0"));
	digits = count_digits_hex_long(n);
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
		result[digits - i++] = base[n % 16];
		n /= 16;
	}
	result[digits] = '\0';
	return (result);
}

int	ft_putchar(char c)
{
	write(STDOUT_FILENO, &c, 1);
	return (1);
}
