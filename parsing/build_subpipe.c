/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_subpipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 10:39:11 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 10:09:12 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//for redir small and big, i != 1 bc there must be a non empty filename behind
// we can use it as check for if we exec'd inside the function or not
static int	redir_big(t_list **lst, char *line, int *i)
{
	char	*str;
	int		res;

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

static int	redir_small(t_list **lst, char *line, int *i)
{
	char	*str;
	int		res;

	str = NULL;
	if (line[*i] == '>')
	{
		res = tokenize_word(line, i, &str, 1);
		if (res <= 0)
			return (res);
		if (!write_token(lst, str))
			return (0);
		return (*i);
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
int	redir_token(t_list **lst, char *line, int *i)
{
	int	res;

	res = redir_big(lst, line, i);
	if (res <= 0 || res > 1)
		return (res);
	res = redir_small(lst, line, i);
	if (res <= 0 || res > 1)
		return (res);
	return (*i);
}

static int	isolate_pipes(char ***subpipe, char *line, int *i, int *k)
{
	int	count;
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
