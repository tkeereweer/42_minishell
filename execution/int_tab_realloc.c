/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_tab_realloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 14:22:48 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 10:27:43 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_int_tab(int **tab)
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

static int	**new_int_tab(int size)
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
	int	i;

	if (!tab)
		return (new_int_tab(size));
	dst = (int **)ft_calloc(size + 1, sizeof(int *));
	if (!dst)
		return (NULL);
	old_len = int_tab_len(tab);
	i = 0;
	while (i < old_len && i < size)
	{
		dst[i] = tab[i];
		i++;
	}
	while (i < size)
	{
		dst[i] = (int *)ft_calloc(2, sizeof(int));
		if (!dst[i])
			return (free_int_tab(dst), NULL);
		i++;
	}
	free(tab);
	return (dst);
}
