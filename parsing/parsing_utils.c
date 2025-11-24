/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:38:12 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/24 16:06:57 by mturgeon         ###   ########.fr       */
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

t_list	*list_error(t_list **list, char *str, t_list **temp)
{
	char	*temp_str;
	int		i;

	if (str)
	{
		if (!ft_strncmp("syntax error\n", str, ft_strlen("syntax error\n")))
		{
			temp_str = (*temp)->next->content->content.str;
			i = 0;
			while (temp_str[i] && !ft_is_whitespace(temp_str[i]))
				i++;
			write(STDERR_FILENO, "syntax error near: '", 20);
			write(STDERR_FILENO, temp_str, i);
			write(STDERR_FILENO, "'\n", 2);
		}
		else
			write(STDERR_FILENO, str, ft_strlen(str));
	}
	ft_lstclear(list, del_linked);
	return (NULL);
}

int	pipeline_list_error(char *str, t_list **temp, int result)
{
	char	*temp_str;
	int		i;

	if (str)
	{
		if (!ft_strncmp("syntax error\n", str, ft_strlen("syntax error\n")))
		{
			temp_str = (*temp)->next->content->content.str;
			i = 0;
			while (temp_str[i] && !ft_is_whitespace(temp_str[i]))
				i++;
			write(STDERR_FILENO, "syntax error near: '", 20);
			write(STDERR_FILENO, temp_str, i);
			write(STDERR_FILENO, "'\n", 2);
			return (result);
		}
		else
			write(STDERR_FILENO, str, ft_strlen(str));
	}
	return (result);
}

int tokenizer_error(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
	return (-1);
}
