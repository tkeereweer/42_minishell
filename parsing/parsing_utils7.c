/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 14:25:36 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 14:28:22 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//function called when heredoc and valid limiter are found
//creates a temporary file in /temp/dev or /dev
//close the file descriptor so file offset resets
//stores filepaths in path_tab, initialized to NULL
char	**heredoc(char **path_tab, char *limiter, int quoted_heredoc
		, int *err_flag)
{
	static int	count = 0;
	int			fd;

	if (!path_tab)
		count = 0;
	if (!limiter)
		return (NULL);
	count++;
	path_tab = temp_filepath(path_tab, count, quoted_heredoc);
	if (!path_tab)
		return (NULL);
	path_tab = try_filepath(path_tab, count);
	if (!path_tab)
		return (free_split(path_tab), NULL);
	fd = open(path_tab[count - 1], O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		return (free_split(path_tab), NULL);
	*err_flag = write_heredoc(limiter, fd);
	if (*err_flag < 0)
		return (free_split(path_tab), NULL);
	return (path_tab);
}

int  valid_char(char *str)
{
	if (!*str)
		return (0);
	if (*str == '|')
		return (0);
	if (is_logic(str))
		return (0);
	if (is_redir(str))
		return (0);
	if (ft_is_whitespace(*str))
		return (0);
	return (1);
}

int	iterate_over_quotes(char *line, int *j)
{
	int	small_quote;
	int	big_quote;

	small_quote = 0;
	big_quote = 0;
	if (line[*j] == '\'' || line[*j] == '"')
	{
		if (line[*j] == '\'')
			small_quote++;
		if (line[*j] == '"')
			big_quote++;
		while (((small_quote % 2 != 0) || (big_quote % 2 != 0)) || valid_char(&line[*j + 1]))
		{
			*j += 1;
			if (!line[*j])
				return (0);
			if (line[*j] == '\'' && (big_quote % 2 == 0))
				small_quote++;
			if (line[*j] == '"' && (small_quote % 2 == 0))
				big_quote++;
		}
	}
	return (1);
}

int return_1_subpipe(char ***subpipe, char *line)
{
	*subpipe = tab_realloc(*subpipe, 1);
	if (!*subpipe)
		return (-1);
	(*subpipe)[0] = ft_strdup(line);
	(*subpipe)[1] = NULL;
	return (1);
}
