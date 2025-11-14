/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:20:12 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/14 16:55:33 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int open_par_token(t_list **list)
{
	t_content   temp_cont;
	t_type      temp_type;
	t_list      *temp;
	t_node		*temp_node;

	temp_type = PAR; 
	temp_cont.parenthesis = '(';
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (-1);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (-1);
	}
	ft_lstadd_back(list, temp);
	return (1);
}

int close_par_token(t_list **list)
{
	t_content   temp_cont;
	t_type      temp_type;
	t_list		*temp;
	t_node		*temp_node;

	temp_type = PAR; 
	temp_cont.parenthesis = ')';
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (-1);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (-1);
	}
	ft_lstadd_back(list, temp);
	return (1);
}

int	pipeline_token(char *str, t_list **list)
{
	t_content   temp_cont;
	t_type      temp_type;
	t_list		*temp;
	t_node		*temp_node;

	temp_type = PIPELINE; 
	temp_cont.str = str;
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (-1);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (-1);
	}
	ft_lstadd_back(list, temp);
	return (1);
}

int	and_token(t_list **list)
{
	t_content   temp_cont;
	t_type      temp_type;
	t_list		*temp;
	t_node		*temp_node;

	temp_type = LOGIC;
	temp_cont.logic = AND;
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (-1);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (-1);
	}
	ft_lstadd_back(list, temp);
	return (1);
}

int	or_token(t_list **list)
{
	t_content   temp_cont;
	t_type      temp_type;
	t_list		*temp;
	t_node		*temp_node;

	temp_type = LOGIC;
	temp_cont.logic = OR;
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (-1);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (-1);
	}
	ft_lstadd_back(list, temp);
	return (1);
}
