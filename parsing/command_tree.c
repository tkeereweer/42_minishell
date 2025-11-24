/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:46:51 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/24 18:26:32 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_cmd_children(t_node *cmd, t_list **pipeline)
{
	t_node	*tmp;

	while (*pipeline != NULL && (*pipeline)->content->type != PIPE)
	{
		if ((*pipeline)->content->type == ARGS)
			cmd->left_child = (*pipeline)->content;
		else if ((*pipeline)->content->type == REDIR)
		{
			tmp = cmd;
			while (tmp->right_child != NULL)
				tmp = tmp->right_child;
			tmp->right_child = (*pipeline)->content;
		}
		*pipeline = (*pipeline)->next;
	}
	if (*pipeline != NULL)
		*pipeline = (*pipeline)->next;
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

int	populate_cmd_tree(t_list **pipeline, t_node *node)
{
	t_content	cont;

	cont.empty = NULL;
	while (*pipeline != NULL)
	{
		node->left_child = node_new(cont, CMD);
		if (node->left_child == NULL)
			return (1);
		node->left_child->parent = node;
		add_cmd_children(node->left_child, pipeline);
		if (*pipeline != NULL)
		{
			if (is_last_pipe(*pipeline) == 0)
			{
				node->right_child = node_new(cont, PIPELINE);
				if (node->right_child == NULL)
					return (1);
				node->right_child->parent = node;
				node = node->right_child;
				if (populate_cmd_tree(pipeline, node) == 1)
					return (1);
			}
			else
			{
				node->right_child = node_new(cont, CMD);
				if (node->right_child == NULL)
					return (1);
				node->right_child->parent = node;
				add_cmd_children(node->right_child, pipeline);
			}
		}
	}
	return (0);
}

void	free_pipeline_list(t_list *pipeline)
{
	t_list	*next;

	while (pipeline != NULL)
	{
		next = pipeline->next;
		if (pipeline->content->type == PIPE)
			free(pipeline->content);
		free(pipeline);
		pipeline = next;
	}
}

void	handle_error_pipeline_list(t_node *node)
{
	if (node->parent == NULL || node->parent->right_child == node)
		ft_putstr_fd("syntax errro near unexpected token: 'newline'\n", STDERR_FILENO);
	else
	{
		ft_putstr_fd("syntax errro near unexpected token: '", STDERR_FILENO);
		if (node->parent->content.logic == AND)
			ft_putstr_fd("&&'\n", STDERR_FILENO);
		else
			ft_putstr_fd("||'\n", STDERR_FILENO);
	}
}

int	create_cmd_trees(t_node *node)
{
	t_list	*pipeline;
	t_list	*start_list;
	int		res;

	// res = 0; // remove if not testing
	if (node == NULL)
		return (0);
	pipeline = NULL;
	create_cmd_trees(node->left_child); // check if function returns 1 ??
	if (node->type == PIPELINE)
	{
		res = pipeline_list(node->content.str, &pipeline);
		if (res <= 0)
		{
			if (res == -1)
				handle_error_pipeline_list(node);
			free_pipeline_list(pipeline);
			return (1);
		}
		start_list = pipeline;
		free(node->content.str);
		if (populate_cmd_tree(&pipeline, node) == 1)
		{
			free_pipeline_list(start_list);
			return (1);
		}
		free_pipeline_list(start_list);
	}
	if (create_cmd_trees(node->right_child) == 1)
		return (1);
	return (0);
}

// int	main(void)
// {
// 	t_list		*lst;
// 	t_list		*start_list;
// 	t_node		*tree;
// 	t_content	cont;

// 	lst = ft_lstnew(node_new(cont, ARGS));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, REDIR)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, PIPE)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, REDIR)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, ARGS)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, REDIR)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, REDIR)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(cont, REDIR)));
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(cont, PIPE)));
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(cont, ARGS)));
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(cont, PIPE)));
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(cont, ARGS)));
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(cont, REDIR)));
// 	tree = node_new(cont, PIPELINE);
// 	start_list = lst;
// 	populate_cmd_tree(&lst, tree);
// 	draw_tree(tree);
// 	free_pipeline_list(start_list);
// 	free_tree(tree);
// 	return (0);
// }
