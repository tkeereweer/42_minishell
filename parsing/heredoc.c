/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:25:50 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/21 11:10:30 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**temp_filepath(char **tmp_name, int count)
{
	char	*tmp_num;
	size_t	len;

	tmp_name = tab_realloc(tmp_name, count);
	if (!tmp_name)
		return (free_split(tmp_name), NULL);
	tmp_name[count] = NULL;
	len = ft_strlen("/Users/maximeturgeon/minishell/temp");
	tmp_num = ft_itoa(count);
	if (!tmp_num)
		return (free_split(tmp_name), NULL);
	tmp_name[count - 1] = (char *)malloc(len + ft_strlen(tmp_num) + 1);
	if (!tmp_name[count - 1])
		return (free(tmp_num), free_split(tmp_name), NULL);
	tmp_name[count - 1][0] = '\0';
	ft_strncat(tmp_name[count - 1], "/Users/maximeturgeon/minishell/temp", len);
	ft_strncat(tmp_name[count - 1], tmp_num, ft_strlen(tmp_num));
	return (free(tmp_num), tmp_name);	
}

static char	**try_filepath(char **filepath, int count)
{
	if (access(filepath[count - 1], F_OK) == 0)
	{
		//if temp file exists, try to delete it to keep going
		if (unlink(filepath[count - 1]) == -1)
		{
			write(STDERR_FILENO, "heredoc failed\n", 15);
			return (NULL);
		}
	}
	return (filepath);	
}

static char	*set_temp_limiter(char *limiter)
{
	char	*tmp_lim;

	tmp_lim = (char *)malloc(sizeof(char) * (ft_strlen(limiter) + 2));
	if (!tmp_lim)
		return (NULL);
	tmp_lim[0] = '\0';
	tmp_lim = ft_strncat(tmp_lim, limiter, ft_strlen(limiter));
	tmp_lim = ft_strncat(tmp_lim, "\n", 1);
	return (tmp_lim);
}
static int	write_heredoc(char *limiter, int fd)
{
	char	*line;
	char	*tmp_lim;

	tmp_lim = set_temp_limiter(limiter);
	if (!tmp_lim)
		return (0);
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (free(tmp_lim), -1);
	while (line && ft_strncmp(line, tmp_lim, ft_strlen(tmp_lim)))
	{
		ft_putstr_fd(line, fd);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	if (line)
		free(line);
	free(tmp_lim);
	return (1);
}
//function called when heredoc and valid limiter are found
//creates a temporary file in /temp/dev or /dev
//close the file descriptor so file offset resets
//stores filepaths in path_tab, initialized to NULL
char	**heredoc(char **path_tab, char *limiter)
{
	static int	count = 0;
	int			fd;

	if (!limiter)
		return (NULL);
	count++;
	path_tab = temp_filepath(path_tab, count);
	if (!path_tab)
		return (NULL);
	path_tab = try_filepath(path_tab, count);
	if (!path_tab)
		return (free(path_tab), NULL);
	//check permission values for open w/ regards of shell functionality?
	fd = open(path_tab[count - 1], O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		return (free(path_tab), NULL);
	if (!write_heredoc(limiter, fd))
		return (free(path_tab), NULL);
	close(fd);
	return (path_tab);
}
