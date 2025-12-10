/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:38:12 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 20:34:03 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

t_list	*syntax_error(t_list **lst)
{
	int		temp;
	t_list *last;

	last = ft_lstlast(*lst);
	temp = last->content->content.logic;
	write(STDERR_FILENO, "minishell: syntax error near unexpected token: ", ft_strlen("minishell: syntax error near unexpected token: "));
	if (temp == 0)
		write(STDERR_FILENO, "'&&'\n", ft_strlen("'&&'\n"));
	else
		write(STDERR_FILENO, "'||'\n", ft_strlen("'||'\n"));
	return (NULL);
}
int	subpipe_error(int code, char **subpipe)
{
	if (subpipe)
		free_split(subpipe);
	return (code);
}
