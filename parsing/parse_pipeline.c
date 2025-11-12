/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:16:56 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/12 21:32:08 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int redir_token(t_list **lst, char *line, int *i)
{
	if (line[*i] == '|')
		if (!pipe_token(lst))
			return (0);
	if (line[*i] == '>')
		if (!write_token(lst))//call them something else like left and right
			return (0);
	if (line[*i] == '<')
		if (!read_token(lst))
			return (0);
	if (line[*i] == '>' && line[*i + 1] && line[*i + 1] == '>')
	{
		if (!append_token(lst))
			return (0);
		*i += 1;
	}
	if (line[*i] == '<' && line[*i + 1] && line[*i + 1] == '<')
	{
		if (!heredoc_token(lst))
			return (0);
		*i += 1;
	}
	return (1);
}

char	*tokenize_word(char *line, int *i)
{
	char	*word;
	int		j;
	int		quote;

	j = *i;
	quote = 0;
	while (line[j])
	{
		if (is_redir_or_quote(&line[j]) && (quote % 2 == 0))
			break;
		if (line[j] == '\'' || line[j] == '"')
			quote++;
		j++;
	}
	if (quote % 2 == 1)
		return (0); //unclosed quote error
	word = ft_substr(line, *i, j - *i);
	if (!word)
		return (NULL);//free shit
	*i = j - 1;
	return (word);
}

//if (lst->prev->content->type = redir | par) & is_alpha(line[i]) ->cmd
//or if no prev token
//check pic and diagnose wtf commands w/ mathijs
int	tokenize_pipe(char *line, t_list **lst)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (ft_is_whitespace(line[i]))
		{
			i++;
			continue;
		}
		if (/*no quotes and first after start, par or redir, then cmd*/)
		{
			if (!cmd_token(tokenize_word(line, &i), lst))
				return (0);
		}
		if (/*prev is cmd*/)
		{
			if (!word_token(tokenize_word(line, &i), lst))
				return (0);
		}
		if (!redir_token(lst, line, &i))
			return (0);
		i++;
	}
}

t_list	*parse_pipeline(t_node *node)
{
	t_list *head;
	
	head = (t_list *)malloc(sizeof(t_list));
	if (!head)
		return (NULL);
	if (!tokenize_pipe(node->content.str, &head) <= 0)
	{

	}
}
