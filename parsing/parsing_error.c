/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:38:12 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 11:27:30 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

t_list	*syntax_error(t_list **lst)
{
	int		temp;
	t_list	*last;
	char	*str;

	str = "minishell: syntax error near unexpected token: ";
	last = ft_lstlast(*lst);
	temp = last->content->content.logic;
	ft_putstr_fd(str, STDERR_FILENO);
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

int	quoted_heredoc_error(int quoted_heredoc)
{
	if (quoted_heredoc == -1)
		return (0);
	return (-1);
}
