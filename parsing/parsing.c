/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 11:50:24 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/12 17:40:56 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	is_sep(char *str)
{
	int	j;
	char *token[5];

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

// int	tokenize_par(char *line, int *i, t_list **list)
// {
// 	char	*pipeline;
// 	int		j;
//     int     k;

// 	j = *i;
//     k = j;
// 	if (!open_par_token(list))//creates one token
// 		return (-1);
//     j++;
// 	while (line[j] != ')')
// 	{
// 		if (!line[j])
// 			return (0);//parse error: unclosed token
//         if (is_sep(&line[j]) && line[j] != '')
//         {
//             pipeline = ft_substr(&line[k + 1], 0, j - k - 1);
//             if (!pipeline)
//                 return (-1); //free shit
//             if (!pipeline_token(pipeline, list))
//                 return (-1);
//             if (line[j] == '&')
//             {
//                 if (and_token(list) == -1)
//                     return (-1);
//             }
//             else
//             {
//                 if (or_token(list) == -1)
//                     return (-1);//free shit
//             }
//             j++;
//             k = j;
//         }
// 		j++;
// 	}
// 	if (!close_par_token(list))
// 		return (-1);
// 	*i = j;
// 	return (1);
// }

int	tokenize_quote(char *line, int *i, t_list **list)
{
	char	*pipeline;
	int		j;

	j = *i;
	while (line[j] != '\'' || line[j] != '"')
	{
		if (!line[j])
			return (0);//parse error: unclosed token
		j++;
	}
	pipeline = ft_substr(line, *i , j + 1 - *i);//inlcude quotes in the pipeline for later operation choice
	if (!pipeline)
		return (-1);//free shit
	if (!pipeline_token(pipeline, list))
		return (-1);
	*i = j;
	return (1);
}

int	tokenize_pipeline(char *line, int *i, t_list **list)
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
	pipeline = ft_substr(line, *i, j);
	if (!pipeline)
		return (-1);//free shit
	if (!pipeline_token(pipeline, list))
		return (-1);
	*i = j - 1;
	return (1);
}

//i is static to output a node once i find it, then go back to the parsing 
int find_node(char *line, t_list **list)
{
	int i;
	int	j;

	*list = (t_list *)malloc(sizeof(t_list));
	if (!*list)
		return (-1);
	*list = NULL;
    i = 0;
	j = 0;
	while (line[i])
	{
		if (ft_is_whitespace(line[i]))
		{
			i++;
			continue;
		}
		else if (line[i] == '(')
		{
			if (open_par_token(list) == -1)
				return (-1);
		}
        else if (line[i] == ')')
		{
			if (close_par_token(list) == -1)
				return (-1);
		}
		else if (line[i] == '&' && line[i + 1] && line[i + 1] == '&')
		{
			if (and_token(list) == -1)
				return (-1);
            i++;
		}
		else if (line[i] == '|' && line[i + 1] && line[i + 1] == '|')
		{
			if (or_token(list) == -1)
				return (-1);
            i++;
		}
		else
		{
			if (tokenize_pipeline(line, &i, list) == -1)
				return (-1);
		}
        i++;
	}
	return (1);
}

int main(void)
{
	t_list *head;
	int result;
    result = find_node("echo a && (echo b || echo 'this is a (test)') || echo c", &head);
	t_list *temp = head;
	int i = 0;

	(void)result;
	while (temp)
	{
		printf("node (%d): ", i);
        if (temp->content->type == 0)
            printf(" LOGIC\n");
        if (temp->content->type == 1)
            printf("%c\n", temp->content->content);
        if (temp->content->type == 2)
            printf("%s\n", temp->content->content);
		i++;
		temp = temp->next;
	}
}
