/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configure_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:17:25 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 09:51:33 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	dup_fds(int fd, t_redir_type kind, int *in_redir, int *out_redir)
{
	if (fd == -1)
		return (-1);
	if (kind == WRITE || kind == APPEND)
	{
		dup2(fd, STDOUT_FILENO);
		*out_redir = 1;
	}
	if (kind == READ || kind == HEREDOC)
	{
		dup2(fd, STDIN_FILENO);
		*in_redir = 1;
	}
	close (fd);
	return (1);
}

static int	prepare_fd_for_exec(int *fd, char *path, char *temp, char *file)
{
	close(*fd);
	*fd = open(path, O_WRONLY | O_TRUNC);
	if (write(*fd, file, ft_strlen(file)) == -1)
		return (free(temp), free(file), -3);
	close(*fd);
	*fd = open(path, O_RDONLY);
	return (0);
}

//check for env variable expansion in heredoc
//flag for quoted heredocs is a 'Q' at end of filepath
static int	expand_heredoc(int *fd, char *path, t_data *data)
{
	char	*temp;
	char	*file;

	file = NULL;
	if (path[ft_strlen(path) - 1] == 'Q')
		return (1);
	temp = get_next_line(*fd);
	if (!temp)
		return (1);
	while (temp)
	{
		file = my_realloc(file, ft_strlen_gnl(file) + ft_strlen(temp) + 1);
		if (!file)
			return (free(temp), -1);
		ft_strncat(file, temp, ft_strlen(temp));
		free(temp);
		temp = get_next_line(*fd);
	}
	if (expand_envvar_str(&file, 0, data, 2) == -1)
		return (free(temp), free(file), -1);
	if (prepare_fd_for_exec(fd, path, temp, file) == -3)
		return (-3);
	return (free(temp), free(file), 1);
}

static int	open_redir(int *fd, t_redir_type kind, t_node *redir, t_data *data)
{
	int	res;

	if (kind == WRITE)
		*fd = open(redir->content.redir.path,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (kind == APPEND)
		*fd = open(redir->content.redir.path,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (kind == READ)
		*fd = open(redir->content.redir.path, O_RDONLY);
	else if (kind == HEREDOC)
	{
		*fd = open(redir->content.redir.path, O_RDONLY);
		res = expand_heredoc(fd, redir->content.redir.path, data);
		if (res < 0)
		{
			close(*fd);
			return (res);
		}
	}
	if (*fd == -1)
		return (*fd);
	return (1);
}

int	configure_redir(t_node *redir, t_data *data, int *in_redir, int *out_redir)
{
	int				fd;
	int				res;
	t_redir_type	kind;

	fd = -1;
	while (redir != NULL)
	{
		res = expand_vars_redir(&(redir->content.redir.path), data);
		if (res == -1)
			return (-1);
		if (res == -4)
			return (ambig_redirect());
		kind = redir->content.redir.kind;
		res = open_redir(&fd, kind, redir, data);
		if (res < 0)
			return (res);
		if (dup_fds(fd, kind, in_redir, out_redir) == -1)
		{
			close(fd);
			return (-2);
		}
		redir = redir->right_child;
	}
	return (1);
}
