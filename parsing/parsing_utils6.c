/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:51:31 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/09 14:03:13 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_path_tab(char **path_tab)
{
	int i;

	if (!path_tab)
		return;
	i = 0;
	while(path_tab[i])
	{
		if (unlink(path_tab[i]) == -1)
		{
			ft_putstr_fd("failed to delete temp file: ", STDERR_FILENO);
			ft_putstr_fd(path_tab[i], STDERR_FILENO);
			write(STDERR_FILENO, "\n", 1);
		}
		free(path_tab[i]);
		path_tab[i] = NULL;
		i++;
	}
	free(path_tab);
	return;
}

//start and end are indexes in the main line
//remove end - start + 1 and add ft_strlen(path)
char	*replace_with_path(char *dest, char *path, int start, int end)
{
	char    *temp;
	size_t  new_len;

	new_len = ft_strlen(dest) - ( end - start + 1) + ft_strlen(path) + 1;
	temp = (char *)ft_calloc(sizeof(char), new_len + 1);
	if (!temp)
		return (NULL);
	ft_strncat(temp, dest, start);
	ft_strncat(temp, path, ft_strlen(path));
	ft_strncat(temp, &dest[end], ft_strlen(&dest[end]));
	return (temp);
}

int	is_last_pipe(t_list *pipeline)
{
	while (pipeline != NULL)
	{
		if (pipeline->content->type == PIPE)
			return (0);
		pipeline = pipeline->next;
	}
	return (1);
}

static int  check_par_usage(t_list **temp)
{
	while ((*temp))
	{
		if ((*temp)->content->type == PAR && (*temp)->content->content.parenthesis == '(')
		{
			if ((*temp)->next && (*temp)->next->content->content.parenthesis == ')')
				return (-3);
			if ((*temp)->prev && (*temp)->prev->content->type == PIPELINE)
				return (-1);
		}
		if ((*temp)->content->type == PAR && (*temp)->content->content.parenthesis == ')')
		{
			if ((*temp)->next && (*temp)->next->content->content.parenthesis == '(')
				return (-3);
			if ((*temp)->next && (*temp)->next->content->type == PIPELINE)
				return (-2);
		}
		(*temp) = (*temp)->next;
	}
	return (1);
}

t_list	*clean_node_list(char **line, char ***path_tab)
{
	int     result;
	t_list	*list;
	t_list	*temp;

	list = NULL;
	result = build_node_list(line, &list, path_tab);
	if (result == 0)
		return (list_error(&list, "minishell: unclosed quotes\n", NULL));
	if (result == -1)
		return (list_error(&list, "minishell: malloc fail somewhere\n", NULL));
	if (result == -2)
		return (list_error(&list, "minishell: dangling logical operator\n", NULL));
	if (result == -3)
		return (syntax_error(&list));
	if (result == -4)
		return (list_error(&list, NULL, NULL));
	if (check_unclosed_par(&list) == -1)
		return (list_error(&list, "minishell: unclosed parenthesis\n", NULL));
	temp = list;
	result = check_par_usage(&temp);
	if (result == -1)
		return (list_error(&list, "minishell: wrong parentheses usage\n", NULL));
	if (result == -2)
		return (list_error(&list, "minishell: syntax error\n", &temp));
	if (result == -3)
		return (list_error(&list, "minishell: syntax error near unexpected token: ')'\n", &temp));
	return (list);
}
