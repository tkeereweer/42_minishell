/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:38:12 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/17 13:29:57 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	is_sep(char *str)
{
	int	j;
	char *token[5];

	token[0] = "(";
	token[1] = ")";
	token[2] = "&&";
	token[3] = "||";
	token[4] = NULL;
	j = 0;
	while (token[j])
	{
		if (!ft_strncmp(str, token[j], ft_strlen(token[j])))
			return (*token[j]);
		j++;
	}
	return (0);
}

char	is_logic(char *str)
{
	int	j;
	char *token[3];

	token[0] = "&&";
	token[1] = "||";
	token[2] = NULL;
	j = 0;
	while (token[j])
	{
		if (!ft_strncmp(str, token[j], ft_strlen(token[j])))
			return (*token[j]);
		j++;
	}
	return (0);
}

char	is_redir(char *str)
{
	int	j;
	char *token[8];

	token[0] = ">";
	token[1] = "<";
	token[2] = ">>";
	token[3] = "<<";
	token[4] = NULL;
	j = 0;
	while (token[j])
	{
		if (!ft_strncmp(str, token[j], ft_strlen(token[j])))
			return (*token[j]);
		j++;
	}
	return (0);
}

void del_linked(t_node *lst)
{
	(void)lst;
}

t_list	*list_error(t_list **list,char *str)
{
	ft_lstclear(list, del_linked);
	if (str)
		write(STDERR_FILENO, str, ft_strlen(str));
	return (NULL);
}

int tokenizer_error(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
	return (-1);
}