/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:14:32 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/18 18:35:46 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *str, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		f(i, &str[i]);
		i++;
	}
}

/*int main(void)
{
	char test[5] = {'H', 'r', 'l', 'T', '!'};
    printf("before: %s\n", test);
	ft_striteri(test, ftest_toupper);
    printf("after: %s\n", test);

}*/