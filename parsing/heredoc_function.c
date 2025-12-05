/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_function.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:25:50 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/05 12:05:30 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**temp_filepath(char **tmp_name, int count, int quoted_heredoc)
{
	char	*tmp_num;
	size_t	len;

	tmp_name = tab_realloc(tmp_name, count);
	if (!tmp_name)
		return (free_split(tmp_name), NULL);
	tmp_name[count] = NULL;
	len = ft_strlen("/tmp/temp");
	if (quoted_heredoc)
		len++;
	tmp_num = ft_itoa(count);
	if (!tmp_num)
		return (free_split(tmp_name), NULL);
	tmp_name[count - 1] = (char *)malloc(len + ft_strlen(tmp_num) + 1);
	if (!tmp_name[count - 1])
		return (free(tmp_num), free_split(tmp_name), NULL);
	tmp_name[count - 1][0] = '\0';
	ft_strncat(tmp_name[count - 1], "/tmp/temp", len);
	ft_strncat(tmp_name[count - 1], tmp_num, ft_strlen(tmp_num));
	if (quoted_heredoc)
		ft_strncat(tmp_name[count - 1], "Q", 1);
	return (free(tmp_num), tmp_name);	
}

static char	**try_filepath(char **filepath, int count)
{
	if (access(filepath[count - 1], F_OK) == 0)
	{
		if (unlink(filepath[count - 1]) == -1)
		{
			write(STDERR_FILENO, "heredoc failed\n", 15);
			return (NULL);
		}
	}
	return (filepath);	
}

static int	write_heredoc(char *limiter, int fd)
{
	char	*line;

    line = readline(">");
	if (!line)
		return (free(limiter), -1);
	while (line && ft_strncmp(line, limiter, ft_strlen(limiter)))//line != eof sent by ctrl D
	{
		ft_putstr_fd(line, fd);
        ft_putstr_fd("\n", fd);
		free(line);
        line = readline(">");
	}
    if (line)
	    free(line);
    close (fd);
	return (1);
}
//function called when heredoc and valid limiter are found
//creates a temporary file in /temp/dev or /dev
//close the file descriptor so file offset resets
//stores filepaths in path_tab, initialized to NULL
char	**heredoc(char **path_tab, char *limiter, int quoted_heredoc)
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
		return (free(path_tab), NULL);
	fd = open(path_tab[count - 1], O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		return (free(path_tab), NULL);
	if (!write_heredoc(limiter, fd))
		return (free(path_tab), NULL);
	return (path_tab);
}
