/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:21:39 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/12 15:46:19 by mkeerewe         ###   ########.fr       */
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

t_node	*find_leftmost_pipeline(t_node *node)
{
	while (node != NULL && node->type != PIPELINE)
		node = node->left_child;
	if (node == NULL)
		return (NULL);
	return (node);
}

t_node	*populate_logic_tree(t_list **list)
{
	t_node	*curr_node;

	curr_node = (*list)->content;
	*list = (*list)->next;
	while ((*list)->content->type != END && (*list)->content->type != PAR)
	{
		curr_node->parent = (*list)->content;
		curr_node->parent->left_child = curr_node;
		curr_node = curr_node->parent;
		*list = (*list)->next;
		if ((*list)->content->type == PAR)
		{
			*list = (*list)->next; 
			curr_node->right_child = populate_logic_tree(list);
			curr_node->right_child->parent = curr_node;
			*list = (*list)->next;
		}
		else if (curr_node->type == LOGIC)
		{
			curr_node->right_child = (*list)->content;
			*list = (*list)->next;
			curr_node->right_child->parent = curr_node;
		}
	}
	return (curr_node);
}

// t_list	*ft_lstnew(t_node *content)
// {
// 	t_list	*node;

// 	node = (t_list *) malloc(sizeof(t_list));
// 	if (node == (void *) 0)
// 		return (node);
// 	node->content = content;
// 	node->next = (void *) 0;
// 	return (node);
// }

// t_list	*ft_lstlast(t_list *lst)
// {
// 	if (lst == (void *) 0)
// 		return (0);
// 	while (lst->next != (void *) 0)
// 	{
// 		lst = lst->next;
// 	}
// 	return (lst);
// }

// void	ft_lstadd_back(t_list **lst, t_list *new)
// {
// 	t_list	*last_node;

// 	if (*lst == (void *) 0)
// 	{
// 		*lst = new;
// 		return ;
// 	}
// 	last_node = ft_lstlast(*lst);
// 	last_node->next = new;
// }

// int	main(void)
// {
// 	t_content	content;
// 	t_list		*lst;
// 	t_node		*tree;

// 	lst = ft_lstnew(node_new(content, PIPELINE));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, LOGIC)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PAR)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PIPELINE)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, LOGIC)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PAR)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PIPELINE)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, LOGIC)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PIPELINE)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PAR)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PAR)));
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, END)));
// 	tree = populate_logic_tree(&lst);
// 	return (0);
// }
