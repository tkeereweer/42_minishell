/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:38:20 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/02 19:41:14 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>
//#include <fcntl.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

void	clean_return(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
	return ;
}

//index of \n, strlen(line) = eol + 1 including eol
//adds space for null byte and for \n
//makes line start after \n.
//end_new + 1 to move in \0.
char	*output_line(char *out, char *line)
{
	size_t	eol;
	char	*temp;
	size_t	end_new;
	int		out_check;

	out_check = 0;
	if (!out)
		out_check = 1;
	eol = 0;
	while (line[eol] != '\n' && line[eol] != '\0')
		eol++;
	if (line[eol] == '\n')
		eol++;
	temp = my_realloc(out, ft_strlen_gnl(out) + eol + 1);
	if (!temp)
		return (NULL);
	out = temp;
	if (out_check == 1)
		*out = '\0';
	ft_strncat(out, line, eol);
	end_new = ft_strlen_gnl(&line[eol]);
	ft_strncpy(line, &line[eol], end_new + 1);
	return (out);
}

//adds length of line to out. empties line into out.
char	*extend_out(char *out, char *line)
{
	char	*temp;
	size_t	out_len;
	size_t	line_len;
	int		out_check;

	out_check = 0;
	if (!out)
		out_check = 1;
	out_len = ft_strlen_gnl(out);
	line_len = ft_strlen_gnl(line);
	temp = my_realloc(out, out_len + line_len + 1);
	if (!temp)
		return (NULL);
	out = temp;
	if (out_check == 1)
		*out = '\0';
	if (line_len == 0)
	{
		ft_strncpy(out, line, out_len);
		if (out_len == 0)
			*out = '\0';
	}
	else
		ft_strncat(out, line, line_len);
	return (out);
}

//reads BUFFER_SIZE into line as long as an EOL or EOF isn't found.
//free line before return null when out is empty and
//reading done as its EOF or error.
//free out bc you loose the ptr if you return NULL (cant free a NULL)
//count == -1: clean_return and free out.
char	*next_content(char *out, char **line, int fd)
{
	ssize_t	count;

	count = 1;
	while (count > 0)
	{
		count = read(fd, *line, BUFFER_SIZE);
		if (count < 1)
		{
			if (ft_strlen_gnl(out) != 0 && count == 0)
			{
				clean_return(line);
				return (out);
			}
			clean_return(line);
			free(out);
			return (NULL);
		}
		(*line)[count] = '\0';
		if (count == BUFFER_SIZE && endof_line(*line) != 1)
			out = extend_out(out, *line);
		else
			return (output_line(out, *line));
	}
	return (out);
}

char	*get_next_line(int fd)
{
	static char	*line;
	char		*out;

	if (fd < 0)
	{
		if (line)
			free(line);
		return (NULL);
	}
	out = NULL;
	if (line == NULL)
	{
		line = (char *) malloc(BUFFER_SIZE + 1);
		if (!line)
			return (NULL);
	}
	else if (endof_line(line) == 1)
		return (output_line(out, line));
	else
		out = extend_out(out, line);
	return (next_content(out, &line, fd));
}
