/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separator_logic.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:25:28 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 11:30:44 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*set_temp(t_list **list, t_list *temp)
{
	if (!temp)
		temp = *list;
	else
		temp = temp->next;
	return (temp);
}

int	tab_len(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

int	is_last_pipe(t_list *pipeline)
{
	while (pipeline != NULL)
	{
		if (pipeline->content->type == PIPE)
			return (0);
		pipeline = pipeline->next;
	}
	return (1);
}

static int	sep_logical_tokenizer(char *line, int *i, t_list **list)
{
	int	j;

	j = 0;
	if (is_logic(&line[*i]))
	{
		if (sep_tokenizer(line, i, list) == -1)
			return (-1);
	}
	else if (line[*i] == '(' )
	{
		if (sep_tokenizer(line, i, list) == -1)
			return (-1);
	}
	else if (line[*i] == ')')
	{
		j = 0;
		while (ft_is_whitespace(line[*i + j]))
			j++;
		if (line[*i + j] && (is_logic(&line[*i + j]) || line[*i + j] == ')'))
			if (sep_tokenizer(line, i, list) == -1)
				return (-1);
	}
	else
		*i += 1;
	return (1);
}

int	separator_logic(char *line, int *i, t_list *temp, t_list **list)
{
	if (is_logic(&line[*i]))
	{
		if (sep_logical_tokenizer(line, i, list) == -1)
			return (-1);
		if (temp && temp->content->type == LOGIC)
			return (-3);
	}
	else if (sep_logical_tokenizer(line, i, list) == -1)
		return (-1);
	*i += 1;
	while (ft_is_whitespace(line[*i]))
		*i += 1;
	return (1);
}
