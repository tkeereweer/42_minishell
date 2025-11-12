/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_node_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 11:50:24 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/12 19:36:08 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	tokenize_pipeline(char *line, int *i, t_list **list)
{
	char	*pipeline;
	int		j;
	int		quote;

	j = *i;
	quote = 0;
	while (line[j])
	{
		if (is_sep(&line[j]) && (quote % 2 == 0))
			break;
		if (line[j] == '\'' || line[j] == '"')
			quote++;
		j++;
	}
	if (quote % 2 == 1)
		return (0); //unclosed quote error
	pipeline = ft_substr(line, *i, j - *i);
	if (!pipeline)
		return (-1);//free shit
	if (!pipeline_token(pipeline, list))
		return (free(pipeline), -1);
	free(pipeline);
	*i = j - 1;
	return (1);
}

static int	sep_tokenizer(char *line, int *i, t_list **list)
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

//i is static to output a node once i find it, then go back to the parsing 
int build_node_list(char *line, t_list **list)
{
	int i;
	int result;

	i = 0;
	while (line[i])
	{
		if (ft_is_whitespace(line[i]))
		{
			i++;
			continue;
		}
		else if (is_sep(&line[i]))
		{
			if (sep_tokenizer(line, &i, list) == -1)
				return (-1);
		}
		else
		{
			result = tokenize_pipeline(line, &i, list);
			if (result <= 0)
				return (result);//returns 0 means unclosed quotes
		}
		i++;
	}
	return (1);
}

int check_unclosed_par(t_list **list)
{
	t_list  *temp;
	temp = *list;
	int     par_count;

	par_count = 0;
	while (temp)
	{
		if (temp->content->type == PAR && temp->content->content.parenthesis == '(')
			par_count++;
		if (temp->content->type == PAR && temp->content->content.parenthesis == ')')
			par_count--;
		temp = temp->next;
	}
	if (par_count != 0)
		return (-1);//unclosed parenthesis
	return (1);
}


t_list	*clean_node_list(char *line)
{
	int result;
	t_list	*list = (t_list *)malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list = NULL;
	result = build_node_list(line, &list);
	if (result <= 0)
	{
		if (result == 0)
			return (list_error(&list, "unclosed quotes\n"));
		list_error(&list, "malloc fail somewhere");
	}
	if (check_unclosed_par(&list) == -1)
		return (list_error(&list, "unclosed parenthesis\n"));
	return (list);
}

int main(void)
{
	t_list *list = clean_node_list("echo a && (echo b || echo 'this is a (test)') || echo c");
	t_list *temp = list;
	int i = 0;

	while (temp)
	{
		printf("node (%d): %d\n", i, temp->content->type);
		i++;
		temp = temp->next;
	}
}
