/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 15:37:52 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/25 13:45:28 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

//%s
size_t	ft_putstr(const char *str)
{
	size_t	len;

	if (str == NULL)
	{
		write(1, "(null)", 6);
		return (6);
	}
	len = ft_strlen(str);
	write(1, str, ft_strlen(str));
	return (len);
}

//%d and %i and %u
size_t	ft_putnbr(int n, t_flag *flag)
{
	char	*result;
	size_t	len;

	result = ft_ltoa(n);
	len = ft_strlen(result);
	if (n >= 0)
	{
		if (flag->plus)
			len += ft_putchar('+');
		if (flag->space)
			len += ft_putchar(' ');
	}
	ft_putstr(result);
	free(result);
	return (len);
}

//%p pointer address 0x[hex] from unsigned int
size_t	ft_putptr(void *ptr)
{
	size_t	len;

	if (ptr == NULL)
		return (ft_putstr("(nil)"));
	len = ft_putstr("0x");
	len += ft_puthex_long((unsigned long)ptr);
	return (len);
}

//for %p therefore no neg values
size_t	ft_puthex_long(unsigned long n)
{
	char	*result;
	size_t	len;

	result = ft_ltohex_long(n);
	len = ft_strlen(result);
	ft_putstr(result);
	free(result);
	return (len);
}

//%X if maj == 1, %x else
size_t	ft_puthex(unsigned int n, int maj, t_flag *flag)
{
	char	*result;
	size_t	len;
	size_t	i;

	i = 0;
	result = ft_ltohex(n);
	len = ft_strlen(result);
	if (flag->hash && maj == 0)
		len += ft_putstr("0x");
	if (flag->hash && maj == 1)
	{
		len += ft_putstr("0X");
	}
	if (maj == 1)
	{
		while (i < len)
		{
			result[i] = ft_toupper(result[i]);
			i++;
		}
	}
	ft_putstr(result);
	free(result);
	return (len);
}
