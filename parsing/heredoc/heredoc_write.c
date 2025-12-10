/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:25:50 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 11:58:19 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern volatile sig_atomic_t	g_signum;

char	**temp_filepath(char **tmp_name, int count, int quoted_heredoc)
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

char	**try_filepath(char **filepath, int count)
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

int	write_heredoc_error(char *line, int fd, int stdin_backup)
{
	close(fd);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	if (handle_signals_parent(0) == 1)
		return (free(line), -1);
	return (free(line), -4);
}

int	write_heredoc(char *limiter, int fd)
{
	char	*line;
	int		stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	if (handle_signals_parent(2) == 1)
		return (-1);
	line = readline(">");
	if (!line || g_signum == SIGINT)
		return (write_heredoc_error(line, fd, stdin_backup));
	while (line && ft_strncmp(line, limiter, ft_strlen(limiter)))
	{
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
		line = readline(">");
	}
	if (!line || g_signum == SIGINT)
		return (write_heredoc_error(line, fd, stdin_backup));
	if (line)
		free(line);
	close (fd);
	close(stdin_backup);
	if (handle_signals_parent(0) == 1)
		return (-1);
	return (1);
}
