/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:20:12 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/12 18:37:44 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int open_par_token(t_list **list)
{
	t_content   temp_cont;
	t_type      temp_type;
	t_list      *temp;

	temp_type = PAR; 
	temp_cont.parenthesis = '(';
	temp = (t_list *)malloc(sizeof(t_list));
	if (!temp)
		return (-1);
	temp->content = node_new(temp_cont, temp_type);
	if (!temp->content)
		return (-1);
	ft_lstadd_back(list, temp);
	return (1);
}

int close_par_token(t_list **list)
{
	t_content   temp_cont;
	t_type      temp_type;
	t_list		*temp;

	temp_type = PAR; 
	temp_cont.parenthesis = ')';
	temp = (t_list *)malloc(sizeof(t_list));
	if (!temp)
		return (-1);
	temp->content = node_new(temp_cont, temp_type);
	if (!temp->content)
		return (-1);
	ft_lstadd_back(list, temp);
	return (1);
}

int	pipeline_token(char *str, t_list **list)
{
	t_content   temp_cont;
	t_type      temp_type;
	t_list		*temp;

	temp_type = PIPELINE; 
	temp_cont.str = str;
	temp = (t_list *)malloc(sizeof(t_list));
	if (!temp)
		return (-1);
	temp->content = node_new(temp_cont, temp_type);
	if (!temp->content)
		return (-1);
	ft_lstadd_back(list, temp);
	return (1);
}

int	and_token(t_list **list)
{
	t_content   temp_cont;
	t_type      temp_type;
	t_list		*temp;

	temp_type = LOGIC;
	temp_cont.logic = AND;
	temp = (t_list *)malloc(sizeof(t_list));
	if (!temp)
		return (-1);
	temp->content = node_new(temp_cont, temp_type);
	if (!temp->content)
		return (-1);
	ft_lstadd_back(list, temp);
	return (1);
}

int	or_token(t_list **list)
{
	t_content   temp_cont;
	t_type      temp_type;
	t_list		*temp;

	temp_type = LOGIC;
	temp_cont.logic = OR;
	temp = (t_list *)malloc(sizeof(t_list));
	if (!temp)
		return (-1);
	temp->content = node_new(temp_cont, temp_type);
	if (!temp->content)
		return (-1);
	ft_lstadd_back(list, temp);
	return (1);
}
