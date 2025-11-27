/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_tab_realloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 14:22:48 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/25 15:12:53 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_int_tab(int **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return ;
}

static int	int_tab_len(int **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

int	**new_int_tab(int size)
{
	int	**dst;
	int	i;
	
	dst = (int **)ft_calloc(size + 1, sizeof(int *));
	if (!dst)
		return (NULL);
	dst[size] = NULL;
	i = 0;
	while (i < size)
	{
		dst[i] = (int *)ft_calloc(2, sizeof(int));
		if (!dst[i])
			return (free_int_tab(dst), NULL);
		i++;
	}
	return (dst);
}

int	**int_tab_realloc(int **tab, int size)
{
	int	**dst;
	int	old_len;

	if (!tab)
		return (new_int_tab(size));
	dst = new_int_tab(size);
	if (!dst)
		return (NULL);
	old_len = int_tab_len(tab);
	ft_memmove(*dst, *tab, old_len * sizeof(int *));
	free(tab);
	return (dst);
}
