/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:46:51 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/15 15:28:59 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_cmd_children(t_node *cmd, t_list **pipeline)
{
	while (*pipeline != NULL && (*pipeline)->content->type != PIPE)
	{
		if ((*pipeline)->content->type == ARGS)
			cmd->left_child = (*pipeline)->content;
		else if ((*pipeline)->content->type == REDIR)
			cmd->right_child = (*pipeline)->content;
		*pipeline = (*pipeline)->next;
	}
	if (*pipeline != NULL)
		*pipeline = (*pipeline)->next;
}

void	populate_cmd_tree(t_list **pipeline, t_node *node)
{
	t_content	cont;

	cont.empty = NULL;
	while (*pipeline != NULL)
	{
		node->left_child = node_new(cont, CMD);
		node->left_child->parent = node;
		add_cmd_children(node->left_child, pipeline);
		if (*pipeline != NULL)
		{
			node->right_child = node_new(cont, PIPELINE);
			node->right_child->parent = node;
			node = node->right_child;
			populate_cmd_tree(pipeline, node);
		}
	}
}

void	free_pipeline_list(t_list *pipeline)
{
	while (pipeline != NULL)
	{
		if (pipeline->content->type == PIPE)
			free(pipeline->content);
		free(pipeline);
		pipeline = pipeline->next;
	}
}

void	create_cmd_trees(t_node *node)
{
	t_list	*pipeline;

	if (node == NULL)
		return ;
	create_cmd_trees(node->left_child);
	if (node->type == PIPELINE)
	{
		// pipeline = parse_pipeline(node);
		free(node->content.str);
		populate_cmd_tree(&pipeline, node);
		free_pipeline_list(pipeline);
	}
	create_cmd_trees(node->right_child);
}

// int	main(void)
// {
// 	t_list		*lst;
// 	t_node		*tree;
// 	t_content	cont;

// 	lst = ft_lstnew(node_new(cont, ARGS));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, REDIR)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, PIPE)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, REDIR)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, ARGS)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, PIPE)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, ARGS)));
// 	tree = node_new(cont, PIPELINE);
// 	populate_cmd_tree(&lst, tree);
// 	free_tree(tree);
// 	return (0);
// }
