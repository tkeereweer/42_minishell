/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:31:03 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/14 11:47:41 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft.h"

typedef struct s_format
{
	int	hash;
	int	space;
	int	plus;
}	t_flag;

size_t	ft_putnbr(int n, t_flag *flag);
size_t	ft_putstr(const char *str);
char	*ft_ltoa(long n);
char	*ft_ltohex(unsigned int n);
char	*ft_ltohex_long(unsigned long n);
size_t	ft_puthex(unsigned int n, int maj, t_flag *flag);
size_t	ft_puthex_long(unsigned long n);
size_t	ft_putptr(void *ptr);
int		ft_putchar(char c);
size_t	ft_putnbr_u(unsigned int n);
size_t	ft_put_substr(const char *str, int stop);
int		print_flag(const char *str, t_flag *flag, va_list *arg, int *i);
int		print_case(const char *str, t_flag *flag, va_list *arg);
int		ft_printf(const char *str, ...);

#endif