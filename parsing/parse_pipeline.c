/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:16:56 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/14 18:31:53 by mturgeon         ###   ########.fr       */
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
	int		quote;
	char	*str;

	quote = 0;
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

char	**tab_realloc(char **tab, int n)
{
	char	**temp;
	int		i;

	temp = (char **)ft_calloc(n + 1, sizeof(char *));
	if (!temp)
		return (NULL);
	temp[n] = NULL;
	if (!tab)
		return (temp);
	i = 0;
	while (tab[i])
		i++;
	ft_memmove(temp, tab, i * sizeof(char *));
	free(tab);
	tab = temp;
	return (tab);
}

//parse and find pipes, caution quotes
int	tokenize_pipe(char *line, t_list **lst)
{
	int		i;
	int		j;
	int		pipe_count;
	char	**subpipe;

	i = 0;
	pipe_count = 0;
	subpipe = NULL;
	while (line[i] && ft_is_whitespace(line[i]))
		i++;
	if (!line[i])
		return (0);//exit code 1
	if (line[i] == '|')
		return (-2);//"syntax error near '|'"
	//count number of pipes
	j = i;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			while (line[i] != '\'' || line[i] != '"')
			{
				if (!line[i])
					return (-1);//unclosed quote error
				i++;
			}
			i++;
		}
		if (line[i] == '|')
		{
			pipe_count++;
			subpipe = tab_realloc(subpipe, pipe_count * 2);	
		}
		i++;
	}

	return (1);
}



		// if (is_redir(line[i]))
		// {	
		// 	j = i;	
		// 	if (!redir_token(lst, line, &i))
		// 		return (0);
		// 	else
		// 	{
		// 		if (!remove_redir(line, j, i))
		// 			return (0);
		// 	}
		// }