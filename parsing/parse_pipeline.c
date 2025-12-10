/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:16:56 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 11:25:23 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>

static int	isolate_redir(char **subpipe, t_list **lst, int *i, int *j)
{
	int	k;

	k = 0;
	while (subpipe[*i][*j])
	{
		if (!iterate_over_quotes(subpipe[*i], j))
			return (subpipe_error(-1, subpipe));
		if (is_redir(&subpipe[*i][*j]))
		{
			k = *j;
			*j = redir_token(lst, subpipe[*i], j);
			if (*j <= 0)
			{
				if (*j == -1 && subpipe[*i + 1])
					return (-2);
				return (*j);
			}
			subpipe[*i] = remove_redir(subpipe[*i], k, *j);
			if (!subpipe[*i])
				return (subpipe_error(0, subpipe));
			*j = -1;
		}
		*j += 1;
	}
	return (1);
}

//passing i as addess to check in error handling
//if next string is null or subpipe
static int	tokenize_subpipe(char **subpipe, t_list **lst)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
	while (subpipe[i])
	{
		j = 0;
		if (subpipe[i][0] == '|')
		{
			if (!pipe_token(lst))
				return (subpipe_error(0, subpipe));
			i++;
			continue ;
		}
		count = isolate_redir(subpipe, lst, &i, &j);
		if (count <= 0)
			return (count);
		if (!arg_token(subpipe[i], lst))
			return (subpipe_error(0, subpipe));
		i++;
	}
	return (1);
}

static void	free_subpipe(char **subpipe)
{
	int	i;

	i = 0;
	while (subpipe[i])
	{
		if (subpipe[i][0] == '|')
			free(subpipe[i]);
		i++;
	}
	free(subpipe);
}

//parse and find pipes, caution quotes
static int	tokenize_pipe(char *line, t_list **lst)
{
	int		i;
	int		result;
	char	**subpipe;

	i = 0;
	subpipe = NULL;
	while (line[i] && ft_is_whitespace(line[i]))
		i++;
	if (!line[i])
		return (0);
	if (line[i] == '|')
		return (-2);
	result = build_subpipe(&subpipe, line, &i);
	if (result <= 0)
		return (result);
	result = tokenize_subpipe(subpipe, lst);
	if (result <= 0)
		return (result);
	free_subpipe(subpipe);
	return (1);
}

int	pipeline_list(char *line, t_list **head)
{
	int		result;
	char	*str;

	str = "minishell: syntax error near unexpected token '|'\n";
	result = tokenize_pipe(line, head);
	if (result == 0)
		return (pipeline_list_error("minishell: malloc fail\n", NULL, result));
	if (result == -2)
		return (pipeline_list_error(str, NULL, result));
	if (result == -1)
		return (-1);
	if (result == -3)
		return (pipeline_list_error("minishell: no command after pipe\n",
				NULL, result));
	return (clean_args_nodes(head));
}
