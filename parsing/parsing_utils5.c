/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:25:28 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/25 10:24:40 by mturgeon         ###   ########.fr       */
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

static int	sep_logical_tokenizer(char *line, int *i, t_list **list/*, t_list **temp*/)
{
	int j; 

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

int check_unclosed_par(t_list **list)
{
	t_list  *temp;
	int     par_count;

	temp = *list;
	par_count = 0;
	while (temp)
	{
		if (temp->content->type == PAR && temp->content->content.parenthesis == '(')
			par_count++;
		if (temp->content->type == PAR && temp->content->content.parenthesis == ')')
			par_count--;
		temp = temp->next;
	}
	if (par_count != 0)
		return (-1);//unclosed parenthesis
	return (1);
}

int	tab_len(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

