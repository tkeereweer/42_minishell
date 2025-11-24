/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 21:04:31 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/24 13:58:08 by mturgeon         ###   ########.fr       */
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
//spaces or redir in file names are accepted if in quotes
//after empty_end, i and j point to first non whitespace char of filepath
//i++ to offset start by 1 when starting w/ quotes
int	tokenize_word(char *line, int *i, char **str, int space)
{
	int		j;
	char	*temp;

	*i += space;
	if (empty_end(line, &j, i) == -1)
		return (-1);//eol after redir --> syntax error near NEXT TOKEN !! ie newline or |, etc
	if (line[j] == '\'' || line[j] == '"')
	{
		if (!iterate_over_quotes(line, &j))
			return (tokenizer_error("unclosed quotes\n"));
		*i += 1;
	}
	else
		while (line[j] && !ft_is_whitespace(line[j]))    
			j++;
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
