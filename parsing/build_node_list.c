/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_node_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 11:50:24 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 10:07:12 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	increment_quote(char **line, int *j,
		int *quote_big, int *quote_small)
{
	if ((*line)[*j] == '\'' && (*quote_big % 2 == 0))
		*quote_small += 1;
	if ((*line)[*j] == '"' && (*quote_small % 2 == 0))
		*quote_big += 1;
	return ;
}

int	check_quote_balance(char **line, int *j, char ***tab)
{
	int		small;
	int		big;
	int		res;

	small = 0;
	big = 0;
	while ((*line)[*j])
	{
		if (is_sep(&(*line)[*j]) && (small % 2 == 0) && (big % 2 == 0))
			break ;
		if ((*line)[*j] == '\'' || (*line)[*j] == '"')
			increment_quote(line, j, &big, &small);
		if ((*line)[*j] == '<' && (*line)[*j + 1] && (*line)[*j + 1] == '<')
		{
			res = set_heredoc(line, j, tab);
			if (res <= 0)
				return (res);
		}
		if (!*line)
			return (-1);
		*j += 1;
	}
	if ((small % 2 == 1) || (big % 2 == 1))
		return (0);
	return (1);
}

static int	tokenize_pipeline(char **line, int *i, t_list **list, char ***tab)
{
	char	*pipeline;
	int		j;
	int		result;

	j = *i;
	result = check_quote_balance(line, &j, tab);
	if (result <= 0)
		return (result);
	pipeline = ft_substr(*line, *i, j - *i);
	if (!pipeline)
		return (-1);
	if (pipeline_token(pipeline, list) == -1)
		return (free(pipeline), -1);
	*i = j;
	return (1);
}

static int	logic_check(t_list *temp)
{
	if (temp->content->type == LOGIC)
	{
		if (!temp->prev)
			return (-3);
		return (-2);
	}
	return (1);
}

//i is static to output a node once i find it, then go back to the parsing
// metachar detection
//l.122: catching two logicals next to e.o. AFTER second one is tokenized
//this helps w/ finding the right syntax error
//temp hasnt been moved so points to "previous" logical
int	build_node_list(char **line, t_list **list, char ***path_tab)
{
	int		i;
	int		result;
	t_list	*temp;

	temp = *list;
	i = 0;
	while ((*line)[i])
	{
		if (is_sep(&(*line)[i]))
		{
			result = separator_logic(*line, &i, temp, list);
			if (result <= 0)
				return (result);
		}
		else
		{
			result = tokenize_pipeline(line, &i, list, path_tab);
			if (result <= 0)
				return (result);
		}
		temp = set_temp(list, temp);
	}
	return (logic_check(temp));
}
