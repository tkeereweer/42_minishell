/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 20:16:56 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/21 11:18:50 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>

static int redir_token(t_list **lst, char *line, int *i)
{
	// int		quote;
	char	*str;

	// quote = 0;
	str = NULL;
	if (line[*i] == '>' && line[*i + 1] && line[*i + 1] == '>')
		if (tokenize_word(line, i, str, 2) == -1 || !append_token(lst, str))
			return (0);
	if (line[*i] == '<' && line[*i + 1] && line[*i + 1] == '<')
		if (tokenize_word(line, i, str, 2) == -1 || !heredoc_token(lst, str))
			return (0);
	if (line[*i] == '>')
		if (tokenize_word(line, i, str, 1) == -1 || !write_token(lst, str))
			return (0);
	if (line[*i] == '<')
		if (tokenize_word(line, i, str, 1) == -1 || !read_token(lst, str))
			return (0);
	return (*i);
}


static int increment_subpipe(char ***subpipe, char *line, int *i, int *j)
{
	static int	pipe_count = 0;
	int len;

	pipe_count++;
	*subpipe = tab_realloc(*subpipe, pipe_count * 2 + 1);
	if (!*subpipe)
		return (0);
	len = tab_len(*subpipe);
	(*subpipe)[len] = ft_substr(line, *j, *i - *j - 1);
	if (!(*subpipe)[len])
		return (subpipe_error(-1, *subpipe));
	(*subpipe)[len + 1] = "|";
	*i += 1;
	while (line[*i] && ft_is_whitespace(line[*i]))
		*i += 1;
	if (!line[*i])
		return (subpipe_error(-3, *subpipe));//no cmd after pipe
	if (line[*i] == '|')
		return (subpipe_error(-2, *subpipe));
	*j = *i;
	return (len);
}

static int	build_subpipe(char ***subpipe, char *line, int *i)
{
	int	j;
	int	k;
	int	count;
	int	quote_count;

	j = *i;
	k = 0;
	quote_count = 0;
	while (line[*i])
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
			quote_count++;
			while (quote_count != 0)
			{
				*i += 1;
				if (!line[*i])
					return (subpipe_error(-1, *subpipe));
				if (line[*i] == '\'' || line[*i] == '"')
					quote_count--;
			}
			*i += 1;
		}
		if (line[*i] == '|')
		{
			count = increment_subpipe(subpipe, line, i, &j);
			if (count < 0)
				return (count);
			k = *i;
		}
		*i += 1;
	}
	if (!*subpipe)
	{
		*subpipe = tab_realloc(*subpipe, 1);
		if (!*subpipe)
			return (-1);
		(*subpipe)[0] = line;
		(*subpipe)[1] = NULL;
		return (1);
	}
	(*subpipe)[count + 2] = ft_substr(line, k, *i - k + 1);
	if (!(*subpipe)[count + 2])
		return (subpipe_error(0, *subpipe));
	return (1);
}

static int	tokenize_subpipe(char **subpipe, t_list **lst)
{
	int	i;
	int	j;
	int	k;
	int	redir_count;
	int	quote_count;

	redir_count = 0;
	quote_count = 0;
	i = 0;
	j = 0;
	k = 0;
	while (subpipe[i])
	{
		j = 0;
		if (subpipe[i][0] == '|')
		{
			if (!pipe_token(lst))
				return (-1);
			i++;
			continue;
		}
		while (subpipe[i][j])
		{
			if (subpipe[i][j] == '\'' || subpipe[i][j] == '"')
			{
				quote_count++;
				while (quote_count != 0)
				{
					j++;
					if (subpipe[i][j] == '\'' || subpipe[i][j] == '"')
						quote_count--;
				}
				j++;
			}
			redir_count = 0;
			if (is_redir(&subpipe[i][j]))
			{
				redir_count = 1;
				k = j;
				j = redir_token(lst, subpipe[i], &j);
				if (!j)
					return (-1);
				subpipe[i] = remove_redir(subpipe[i], k, j);
				if (!subpipe[i])
					return (subpipe_error(0, subpipe));
				if (!arg_token(subpipe[i], lst))
					return (subpipe_error(0, subpipe));
				break;
			}
			j++;
		}
		if (!redir_count)
			if (!arg_token(subpipe[i], lst))
				return (subpipe_error(0, subpipe));
		i++;
	}
	return (1);
}

//parse and find pipes, caution quotes
int	tokenize_pipe(char *line, t_list **lst)
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

t_list *pipeline_list(char *line)
{
	t_list	*head;
	int		result;
	t_list	*temp;
	char 	**temp_tab;
	char	*temp_str;

	head = NULL;
	result = tokenize_pipe(line, &head);
	if (result == 0)
		list_error(&head, "malloc fail");
	if (result == -2)
		list_error(&head, "syntax error near '|'");
	if (result == -1)
		list_error(&head, NULL);
	temp = head;
	while (temp)
	{
		if (temp->content->type == ARGS)
		{
			temp_str = temp->content->content.str;
			temp_tab = args_tab(temp_str);
			if (!temp_tab)
				return (NULL);
			temp->content->content.tab = temp_tab;
		}
		temp = temp->next;
	}	
	return (head);
}



// int main(void)
// {
// 	t_list *list;
// 	t_list *temp;

// 	list = NULL;
// 	if (!tokenize_pipe("cat '< file1\" | wc -c | grep e > file2 | echo out", &list))
// 	{
// 		printf("shit went worng");
// 		return (1);
// 	}
// 	temp = list;
// 	int i =0;
// 	while (temp)
// 	{
// 		printf("node (%d): %d\n", i, temp->content->type);
// 		i++;
// 		temp = temp->next;
// 	}
// }
