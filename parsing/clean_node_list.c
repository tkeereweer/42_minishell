/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_node_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:51:31 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 10:23:38 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_path_tab(char **path_tab)
{
	int	i;

	if (!path_tab)
		return ;
	i = 0;
	while (path_tab[i])
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
	return ;
}

int	check_unclosed_par(t_list **list)
{
	t_list	*temp;
	int		par_count;

	temp = *list;
	par_count = 0;
	while (temp)
	{
		if (temp->content->type == PAR
			&& temp->content->content.parenthesis == '(')
			par_count++;
		if (temp->content->type == PAR
			&& temp->content->content.parenthesis == ')')
			par_count--;
		temp = temp->next;
	}
	if (par_count != 0)
		return (-1);
	return (1);
}

static int	check_par_usage(t_list **temp)
{
	while ((*temp))
	{
		if ((*temp)->content->type == PAR
			&& (*temp)->content->content.parenthesis == '(')
		{
			if ((*temp)->next
				&& (*temp)->next->content->content.parenthesis == ')')
				return (-3);
			if ((*temp)->prev && (*temp)->prev->content->type == PIPELINE)
				return (-1);
		}
		if ((*temp)->content->type == PAR
			&& (*temp)->content->content.parenthesis == ')')
		{
			if ((*temp)->next
				&& (*temp)->next->content->content.parenthesis == '(')
				return (-3);
			if ((*temp)->next && (*temp)->next->content->type == PIPELINE)
				return (-2);
		}
		(*temp) = (*temp)->next;
	}
	return (1);
}

static t_list	*par_usage(t_list *list)
{
	t_list	*tmp;
	int		result;

	tmp = list;
	result = check_par_usage(&tmp);
	if (result == -1)
		return (list_error(&list,
				"minishell: wrong parentheses usage\n", NULL));
	if (result == -2)
		return (list_error(&list,
				"minishell: syntax error\n", &tmp));
	if (result == -3)
		return (list_error(&list,
				"minishell: syntax error near unexpected token: ')'\n", &tmp));
	return (list);
}

t_list	*clean_node_list(char **line, char ***path_tab, t_data *data)
{
	int		result;
	t_list	*list;

	list = NULL;
	result = build_node_list(line, &list, path_tab);
	if (result == 0)
		return (list_error(&list, "minishell: unclosed quotes\n", NULL));
	if (result == -1)
		return (list_error(&list,
				"minishell: malloc fail somewhere\n", NULL));
	if (result == -2)
		return (list_error(&list,
				"minishell: dangling logical operator\n", NULL));
	if (result == -3)
		return (syntax_error(&list));
	if (result == -4)
	{
		data->exit_status = 130;
		return (list_error(&list, NULL, NULL));
	}
	if (check_unclosed_par(&list) == -1)
		return (list_error(&list, "minishell: unclosed parenthesis\n", NULL));
	return (par_usage(list));
}
