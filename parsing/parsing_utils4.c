/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 09:56:21 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/04 16:47:17 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int clean_args_nodes(t_list **head)
{
	t_list	*temp;
	char 	**temp_tab;
	char	*temp_str;

	temp = *head;
	while (temp)
	{
		if (temp->content->type == ARGS)
		{
			temp_str = temp->content->content.str;
			temp_tab = args_tab(temp_str);
			if (!temp_tab)
				return (0);
			temp->content->content.tab = temp_tab;
		}
		temp = temp->next;
	}
	return (1);
}

int increment_subpipe(char ***subpipe, char *line, int *i, int *j)
{
	static int	pipe_count = 0;
	int len;

	pipe_count++;
	*subpipe = tab_realloc(*subpipe, pipe_count * 2 + 1);
	if (!*subpipe)
		return (0);
	len = tab_len(*subpipe);
	(*subpipe)[len] = ft_substr(line, *j, *i - *j);//duplicates part of line before pipe
	if (!(*subpipe)[len])
		return (subpipe_error(-1, *subpipe));
	(*subpipe)[len + 1] = ft_strdup("|");
	if (!(*subpipe)[len + 1])
		return (subpipe_error(-1, *subpipe));
	*i += 1;
	while (line[*i] && ft_is_whitespace(line[*i]))
		*i += 1;
	if (!line[*i])
		return (subpipe_error(-3, *subpipe));
	if (line[*i] == '|')
		return (subpipe_error(-2, *subpipe));
	*j = *i;
	return (len);
}

int	sep_tokenizer(char *line, int *i, t_list **list)
{
	if (line[*i] == '(')
	{
		if (open_par_token(list) == -1)
			return (-1);
	}
	else if (line[*i] == ')')
	{
		if (close_par_token(list) == -1)
			return (-1);
	}
	else if (line[*i] == '&' && line[*i + 1] && line[*i + 1] == '&')
	{
		if (and_token(list) == -1)
			return (-1);
		*i += 1;
	}
	else if (line[*i] == '|' && line[*i + 1] && line[*i + 1] == '|')
	{
		if (or_token(list) == -1)
			return (-1);
		*i += 1;
	}
	return (1);
}

void del_linked(t_node *lst)
{
	(void)lst;
}

int	subpipe_error(int code, char **subpipe)
{
	if (subpipe)
		free_split(subpipe);
	return (code);
}
