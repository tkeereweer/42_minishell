/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:30:35 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/14 11:58:56 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

int	ft_printf(const char *str, ...)
{
	va_list	arglist;
	t_flag	format;
	int		i;
	size_t	len;

	va_start(arglist, str);
	format.space = 0;
	format.hash = 0;
	format.plus = 0;
	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '%')
			len += print_flag(str, &format, &arglist, &i);
		else
			len += ft_putchar(str[i]);
		i++;
	}
	va_end(arglist);
	return ((int) len);
}

int	print_case(const char *str, t_flag *flag, va_list *arg)
{
	if (*str == 'c')
		return (ft_putchar(va_arg(*arg, int)));
	if (*str == 's')
		return (ft_putstr(va_arg(*arg, char *)));
	if (*str == 'p')
		return (ft_putptr(va_arg(*arg, void *)));
	if (*str == 'd' || *str == 'i')
		return (ft_putnbr(va_arg(*arg, long), flag));
	if (*str == 'u')
		return (ft_putnbr_u(va_arg(*arg, unsigned int)));
	if (*str == 'x')
		return (ft_puthex(va_arg(*arg, unsigned int), 0, flag));
	if (*str == 'X')
		return (ft_puthex(va_arg(*arg, unsigned int), 1, flag));
	if (*str == '%')
		return (ft_putchar('%'));
	return (-1);
}

int	print_flag(const char *str, t_flag *flag, va_list *arg, int *i)
{
	if (str[*i + 1] == '#' || str[*i + 1] == ' ' || str[*i + 1] == '+')
	{
		if (str[*i + 1] == '#')
			flag->hash = 1;
		if (str[*i + 1] == '+')
			flag->plus = 1;
		else
			flag->space = 1;
		(*i)++;
	}
	(*i)++;
	return (print_case(&str[*i], flag, arg));
}
