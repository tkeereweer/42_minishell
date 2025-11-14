/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 21:04:31 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/12 21:16:04 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cmd_token(char *word, t_list **lst)
{
	t_content	temp_cont;
	t_type		temp_type;
	t_list		*temp;

	temp = (t_list *)malloc(sizeof(t_list));
	if (!temp)
		return (0);
	temp_cont.str = word;
	temp_type = CMD;
	temp->content = node_new(temp_cont, temp_type);
	if (!temp->content)
	{
		free(temp);
		return (0);
	}
	ft_lstadd_back(lst, temp);
	return (1);	
}

int	arg_token(char *word, t_list **lst)
{
	t_content	temp_cont;
	t_type		temp_type;
	t_list		*temp;

	temp = (t_list *)malloc(sizeof(t_list));
	if (!temp)
		return (0);
	temp_cont.str = word;
	temp_type = ARGS;
	temp->content = node_new(temp_cont, temp_type);
	if (!temp->content)
	{
		free(temp);
		return (0);
	}
	ft_lstadd_back(lst, temp);
	return (1);	
}