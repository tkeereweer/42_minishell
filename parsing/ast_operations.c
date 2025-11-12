/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:21:39 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/12 19:41:51 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_node	*node_new(t_content content, t_type type)
{
	t_node	*new;

	new = (t_node *) malloc(sizeof(t_node));
	if (new == NULL)
		return (NULL);
    new->type = type;
	if (type == LOGIC)
		new->content.logic = content.logic;
	else if (type == PIPELINE)
		new->content.str = content.str;
	else if (type == CMD)
		new->content.str = content.str;
	else if (type == ARGS)
		new->content.str = content.str;
	else
		new->content.redir = content.redir;
    new->left_child = NULL;
	new->right_child = NULL;
	new->parent = NULL;
	return (new);
}

// int	main(void)
// {
// 	t_content content;
// 	t_node *node;

// 	content.logic = AND;
// 	node = node_new(content, LOGIC);
// 	return (0);
// }
