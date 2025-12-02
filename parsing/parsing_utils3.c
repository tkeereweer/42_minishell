/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 10:39:11 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/02 18:02:38 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int  valid_char(char *str)
{
	if (!*str)
		return (0);
	if (*str == '|')
		return (0);
	if (is_logic(str))
		return (0);
	if (is_redir(str))
		return (0);
	if (ft_is_whitespace(*str))
		return (0);
	return (1);
}

int	iterate_over_quotes(char *line, int *j)
{
	int	small_quote;
	int	big_quote;

	small_quote = 0;
	big_quote = 0;
	if (line[*j] == '\'' || line[*j] == '"')
	{
		if (line[*j] == '\'')
			small_quote++;
		if (line[*j] == '"')
			big_quote++;
		while (((small_quote % 2 != 0) || (big_quote % 2 != 0)) || valid_char(&line[*j + 1]))
		{
			*j += 1;
			if (!line[*j])
				return (0);
			if (line[*j] == '\'' && (big_quote % 2 == 0))
				small_quote++;
			if (line[*j] == '"' && (small_quote % 2 == 0))
				big_quote++;
		}
	}
	return (1);
}
//for redir small and big, i != 1 bc there must be a non empty filename behind
// we can use it as check for if we exec'd inside the function or not
static int  redir_big(t_list **lst, char *line, int *i)
{
	char	*str;
	int     res;

	str = NULL;
	if (line[*i] == '>' && line[*i + 1] && line[*i + 1] == '>')
	{
		res = tokenize_word(line, i, &str, 2);
		if (res <= 0)
			return (res);
		if (!append_token(lst, str))
			return (0);
		return (*i);
	}
	if (line[*i] == '<' && line[*i + 1] && line[*i + 1] == '<')
	{
		res = tokenize_word(line, i, &str, 2);
		if (res <= 0)
			return (res);
		if (!heredoc_token(lst, str))
			return (0);
		return (*i);
	}
	return (1);
}

static int  redir_small(t_list **lst, char *line, int *i)
{
 	char	*str;
	int     res;

	str = NULL;
	if (line[*i] == '>')
	{
		res = tokenize_word(line, i, &str, 1);
		if (res <= 0)
			return (res);
		if (!write_token(lst, str))
			return (0);
		return(*i);
	}
	if (line[*i] == '<')
	{
		res = tokenize_word(line, i, &str, 1);
		if (res <= 0)
			return (res);
		if (!read_token(lst, str))
			return (0);
		return (*i);
	}
	return (1);   
}

//returns error code or value of *i
int redir_token(t_list **lst, char *line, int *i)
{
	int res;

	res = redir_big(lst, line, i);
	if (res <= 0 || res > 1)
		return (res);
	res = redir_small(lst, line, i);
	if (res <= 0 || res > 1)
		return (res);
	return (*i);
}

int return_1_subpipe(char ***subpipe, char *line)
{
	*subpipe = tab_realloc(*subpipe, 1);
	if (!*subpipe)
		return (-1);
	(*subpipe)[0] = line;
	(*subpipe)[1] = NULL;
	return (1);
}

static int	isolate_pipes(char ***subpipe, char *line, int *i, int *k)
{
	int count;
	int	j;

	j = *i;
	count = 1;
	while (line[*i])
	{
		if (!iterate_over_quotes(line, i))
			return (subpipe_error(-1, *subpipe));
		if (line[*i] == '|')
		{
			count = increment_subpipe(subpipe, line, i, &j);
			if (count < 0)
				return (count);
			*k = *i;
		}
		else
			*i += 1;
	}
	return (count);
}

int	build_subpipe(char ***subpipe, char *line, int *i)
{
	int	k;
	int	count;

	k = 0;
	count = isolate_pipes(subpipe, line, i, &k);
	if (count < 0)
		return (count);
	if (!*subpipe)
		return (return_1_subpipe(subpipe, line));
	(*subpipe)[count + 2] = ft_substr(line, k, *i - k + 1);
	if (!(*subpipe)[count + 2])
		return (subpipe_error(0, *subpipe));
	return (1);
}
