/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:16:56 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/16 12:11:01 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <linux/limits.h>

//only handle quotes if at begining of filepath
//space defines offset to start after > or >>
static int	tokenize_word(char *line, int *i, char *str, int space)
{
	int		j;
	int		quote;

	*i += space;
	quote = 0;
	if (empty_end(line, &j, i) == -1)
		return (-1);
	if (line[j] == '\'' || line[j] == '"')
		quote++;
	while (line[j])
	{
		if (line[j] == '\'' || line[j] == '"')
			quote++;
		if (is_redir(&line[j]) || (quote % 2 == 0))
			break;
		if (ft_is_whitespace(line[j]) && quote == 0)
			break;
		j++; 
	}
	if (quote % 2 == 1)
		return (tokenizer_error("unclosed quotes\n"));
	str = ft_substr(&line[j], 0, j - *i);
	if (!str)
		return (tokenizer_error("malloc fail\n"));
	*i = j - 1;
	return (1);
}

static int redir_token(t_list **lst, char *line, int *i)
{
	// int		quote;
	char	*str;

	// quote = 0;
	str = NULL;
	if (line[*i] == '>')
		if (tokenize_word(line, i, str, 1) == -1 || !write_token(lst, str))
			return (0);
	if (line[*i] == '<')
		if (tokenize_word(line, i, str, 1) == -1 || !read_token(lst, str))
			return (0);
	if (line[*i] == '>' && line[*i + 1] && line[*i + 1] == '>')
		if (tokenize_word(line, i, str, 2) == -1 || !append_token(lst, str))
			return (0);
	if (line[*i] == '<' && line[*i + 1] && line[*i + 1] == '<')
		if (tokenize_word(line, i, str, 2) == -1 || !heredoc_token(lst, str))
			return (0);
	return (1);
}
//deletes chars from start to end INCLUDED 
//overwrites them with the end
char	*remove_redir(char *str, int start, int end)
{
	char	*dest;
	size_t	dest_len;
	int		i;
	int		j;

	dest_len = ft_strlen(str) - (start - end);
	dest = (char *)malloc(dest_len);
	if (!dest)
		return (NULL);
	ft_strncpy(dest, str, start);
	i = end;
	j = start;
	while (str[i])
	{
		dest[j] = str[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	free(str);
	return (dest);
}

//if (lst->prev->content->type = redir | par) & is_alpha(line[i]) ->cmd
//or if no prev token
//check pic and diagnose wtf commands w/ mathijs
int	tokenize_pipe(char *line, t_list **lst)
{
	int		j;
	int		i;
	int		k;
	char	**pipeline;

	i = 0;
	while (ft_is_whitespace(line[i]))
		i++;
	if (line[i] == '|')
		return (-2);//pipe syntax error
	pipeline = ft_split(&line[i], '|');
	if (!pipeline)
		return (0);
	j = 0;
	while (pipeline[j])
	{
		i = 0;
		while (pipeline[j][i])
		{
			if (ft_is_whitespace(line[i]))
				i++;
			k = i;
			if (!redir_token(lst, line, &i))
				return (0);
			else
			{
				if (!remove_redir(pipeline[j], k, i))
					return (0);
			}
			i++;
		}
		if (!pipe_token(lst))
			return (0);
		j++;
	}
	if ((*lst)->content->type == PIPELINE)
		return (-2);
	return (1);
}

// t_list	*parse_pipeline(t_node *node)
// {
// 	t_list *head;
	
// 	head = (t_list *)malloc(sizeof(t_list));
// 	if (!head)
// 		return (NULL);
// 	if (!tokenize_pipe(node->content.str, &head) <= 0)
// 	{

// 	}
// }
