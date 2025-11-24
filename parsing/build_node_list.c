/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_node_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 11:50:24 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/24 14:22:25 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int  check_quote_balance(char *line, int *j, char **tab)
{
	int		quote_small;
	int		quote_big;

	quote_small = 0;
	quote_big = 0;
	while (line[*j])
	{
		if (is_sep(&line[*j]) && (quote_small % 2 == 0) && (quote_big % 2 == 0))
			break;
		if (line[*j] == '\'' || line[*j] == '"')
		{
			if (line[*j] == '\'' && (quote_big % 2 == 0))
				quote_small++;
			if (line[*j] == '"' && (quote_small % 2 == 0))
				quote_big++;
		}
		if (line[*j] == '<' && line[*j +  1] && line[*j + 1] == '<')
			if (!set_heredoc(&line, j, tab))
				return (-1);
		*j += 1;
	}
	if ((quote_small % 2 == 1) || (quote_big % 2 == 1))
		return (0); //unclosed quote error RETURN CHARACTER WITH UNCLOSED TOKEN
	return (1);
}

static int	tokenize_pipeline(char *line, int *i, t_list **list, char **tab)
{
	char	*pipeline;
	int		j;
	int     result;

	j = *i;
	result = check_quote_balance(line, &j, tab);
	if (result <= 0)
		return (result);
	pipeline = ft_substr(line, *i, j - *i);
	if (!pipeline)
		return (-1);//free shit
	if (!pipeline_token(pipeline, list))
		return (free(pipeline), -1);
	*i = j;
	return (1);
}


//i is static to output a node once i find it, then go back to the parsing
// metachar detection
//l.122: catching two logicals next to e.o. AFTER second one is tokenized
//this helps w/ finding the right syntax error
//temp hasnt been moved so points to "previous" logical
int build_node_list(char *line, t_list **list, char **path_tab)
{
	int i;
	int result;
	t_list	*temp;

	temp = *list;
	i = 0;
	while (line[i])
	{
		if (is_sep(&line[i]))
		{
			result = separator_logic(line, &i, temp, list);
			if (result <= 0)
				return (result);
		}
		else
		{
			result = tokenize_pipeline(line, &i, list, path_tab);
			if (result <= 0)
				return (result);//returns 0 means unclosed quotes
		}
		temp = set_temp(list, temp);
	}
	if (temp->content->type == LOGIC)
		return (-2);
	return (1);
}

static int  check_par_usage(t_list **temp)
{
	while ((*temp))
	{
		if ((*temp)->content->type == PAR && (*temp)->content->content.parenthesis == '(')
		{
			if ((*temp)->prev && (*temp)->prev->content->type == PIPELINE)
				return (-1);//wrong par usage
		}
		if ((*temp)->content->type == PAR && (*temp)->content->content.parenthesis == ')')
		{
			if ((*temp)->next && (*temp)->next->content->type == PIPELINE)
				return (-2);//syntax error near **first word of next token**
		}
		(*temp) = (*temp)->next;
	}
	return (1);
}

t_list	*clean_node_list(char *line, char **path_tab)
{
	int result;
	t_list	*list;
	t_list	*temp;

	list = NULL;
	result = build_node_list(line, &list, path_tab);
	if (result == 0)
		return (list_error(&list, "unclosed quotes\n", NULL));
	if (result == -1)
		return (list_error(&list, "malloc fail somewhere\n", NULL));
	if (result == -2)
		return (list_error(&list, "dangling logical operator\n", NULL));
	if (result == -3)
		return (syntax_error(&list)); 
	if (check_unclosed_par(&list) == -1)
		return (list_error(&list, "unclosed parenthesis\n", NULL));
	temp = list;
	result = check_par_usage(&temp);
	if (result == -1)
		return (list_error(&list, "wrong parentheses usage\n", NULL));
	if (result == -2)
		return (list_error(&list, "syntax error\n", &temp));
	return (list);
}
