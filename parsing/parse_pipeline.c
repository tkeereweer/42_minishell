/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:16:56 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/28 08:40:30 by mkeerewe         ###   ########.fr       */
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
//passing i as addess to check in error handling if next string is null or subpipe
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
				return (0);
			i++;
			continue;
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
		return (0);//exit code 1
	if (line[i] == '|')
		return (-2);//"syntax error near '|'"
	result = build_subpipe(&subpipe, line, &i);
	if (result <= 0)
		return (result);
	result = tokenize_subpipe(subpipe, lst);
	if (result <= 0)
		return (result);
	return (1);
}

int pipeline_list(char *line, t_list **head)
{
	int		result;
    // t_list  *temp;

    // temp = *head;
	result = tokenize_pipe(line, head);
	if (result == 0)
		return (pipeline_list_error("malloc fail\n", NULL, result));
	if (result == -2)
		return (pipeline_list_error("syntax error near '|'\n", NULL, result));
	if (result == -1)
		return (-1);
	if (result == -3)
		return (pipeline_list_error("no command after pipe\n", NULL, result));
	return (clean_args_nodes(head));
}
