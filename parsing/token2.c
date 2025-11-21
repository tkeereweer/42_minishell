/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 21:04:31 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/18 16:14:15 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	arg_token(char *word, t_list **lst)
{
    t_node      *temp_node;
	t_content	temp_cont;
	t_type		temp_type;
	t_list		*temp;

	temp_cont.str = word;
	temp_type = ARGS;
	temp_node = node_new(temp_cont, temp_type);
	if (!temp_node)
		return (0);
	temp = ft_lstnew(temp_node);
	if (!temp)
	{
		free(temp_node);
		return (0);
	}
	ft_lstadd_back(lst, temp);
	return (1);	
}

//only handle quotes if at begining of filepath
//space defines offset to start after > or >>
int	tokenize_word(char *line, int *i, char **str, int space)
{
	int		j;
	int		quote;
	char	*temp;

	*i += space;
	quote = 0;
	if (empty_end(line, &j, i) == -1)
		return (-1);//eol after redir --> syntax error near >
	if (line[j] == '\'' || line[j] == '"')
		quote++;
	while (line[j])
	{
		if (line[j] == '\'' || line[j] == '"')
			quote++;
		if (is_redir(&line[j]) && (quote % 2 == 0))
			break;
		if (ft_is_whitespace(line[j]) && quote == 0)
			break;
		j++; 
	}
	if (quote % 2 == 1)
		return (tokenizer_error("unclosed quotes\n"));
	*str = ft_substr(&line[*i], 0, j - *i);
	if (!*str)
		return (tokenizer_error("malloc fail\n"));
	temp = ft_strtrim(*str, " \t\n\v\f\r");
	if (!temp)
		return (free(str), tokenizer_error("malloc fail\n"));
	free(*str);
	*str = temp;
	*i = j;
	return (1);
}
