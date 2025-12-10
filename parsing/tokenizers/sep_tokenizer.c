/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_tokenizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 09:56:21 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 11:58:24 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	increment_subpipe(char ***subpipe, char *line, int *i, int *j)
{
	static int	pipe_count = 0;
	int			len;

	pipe_count++;
	*subpipe = tab_realloc(*subpipe, pipe_count * 2 + 1);
	if (!*subpipe)
		return (0);
	len = tab_len(*subpipe);
	(*subpipe)[len] = ft_substr(line, *j, *i - *j);
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

char	is_sep(char *str)
{
	int		j;
	char	*token[5];

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
	int		j;
	char	*token[3];

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

t_make_tab	init_struct(char *str)
{
	t_make_tab	utils;

	utils.i = 0;
	utils.j = 0;
	utils.word_count = 0;
	utils.str = str;
	utils.tab = NULL;
	while (ft_is_whitespace(str[utils.i]))
		utils.i++;
	return (utils);
}
