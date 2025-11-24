/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:20:27 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/20 09:20:17 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_unset(char **tab, t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (tab[i] != NULL)
	{
		j = 0;
		while (data->env[j] != NULL)
		{
			if (ft_strncmp(tab[i], data->env[j], ft_strlen(tab[i])) == 0 && data->env[j][ft_strlen(tab[i])] == '=')
			{
				free(data->env[j]);
				break ;
			}
			j++;
		}
		while (data->env[j] != NULL)
		{
			data->env[j] = data->env[j + 1];
			j++;
		}
		i++;
	}
}
