/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:10:27 by mturgeon          #+#    #+#             */
/*   Updated: 2025/10/21 15:50:19 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//whitespace, optional + or -, change sign accordingly
int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	sign_num;

	sign = 0;
	result = 0;
	sign_num = 0;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	while (*str == 43 || *str == 45)
	{
		if (*str == 45)
			sign++;
		sign_num++;
		str++;
	}
	while (ft_isnum(*str) && sign_num < 2)
	{
		result = result * 10 + *str - '0';
		str++;
	}
	if (sign)
		return (-result);
	return (result);
}
/*
int main(void)
{
	char *test = "\t\v\f\r\n \f-06050";
	printf("my result: %d\n, stdlib result: %d\n",
			ft_atoi(test), atoi(test));

}*/
