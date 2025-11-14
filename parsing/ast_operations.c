/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:21:39 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/14 16:59:41 by mkeerewe         ###   ########.fr       */
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

void	clean_par(t_list **list)
{
	int		par_cnt;
	t_list	*tmp;

	par_cnt = 1;
	tmp = *list;
	// free((*list)->content);
	*list = (*list)->next;
	while (par_cnt != 0 && tmp->next != NULL)
	{
		tmp = tmp->next;
		if (tmp->content->type == PAR && tmp->content->content.parenthesis == '(')
			par_cnt++;
		else if (tmp->content->type == PAR && tmp->content->content.parenthesis == ')')
			par_cnt--;
	}
	tmp->prev->next = tmp->next;
	if (tmp->next != NULL)
		tmp->next->prev = tmp->prev;
	free(tmp->content);
	free(tmp);
}

t_list	*go_to_eob(t_list *list)
{
	int	par_cnt;

	par_cnt = 1;
	list = list->next;
	while (par_cnt != 0 && list != NULL)
	{
		if (list->content->type == PAR && list->content->content.parenthesis == '(')
			par_cnt++;
		else if (list->content->type == PAR && list->content->content.parenthesis == ')')
			par_cnt--;
		list = list->next;
	}
	return (list);
}

t_node	*populate_logic_tree(t_list *list)
{
	t_node	*node;
	t_list	*start_of_block;

	while (list->content->type == PAR)
		clean_par(&list);
	node = list->content;
	list = list->next;
	while (list != NULL)
	{
		node->parent = list->content;
		node->parent->left_child = node;
		node = node->parent;
		list = list->next;
		if (list->content->type == PAR)
		{
			start_of_block = list;
			list = go_to_eob(list);
			if (list != NULL)
			{
				list->prev->next = NULL;
				list->prev = NULL;
			}
			node->right_child = populate_logic_tree(start_of_block);
		}
		else if (list->content->type == PIPELINE)
		{
			node->right_child = list->content;
			list = list->next;
		}
	}
	return (node);
}

t_node	*create_logic_tree(t_list *list)
{
	t_node	*ret;
	t_list	*tmp;
	int		first_par;

	ret = populate_logic_tree(list);
	first_par = 0;
	while (list != NULL)
	{
		tmp = list;
		list = list->next;
		if (tmp->content->type == PAR && first_par == 0)
		{
			free(tmp->content);
			first_par = 1;
		}
		free(tmp);
	}
	return (ret);
}

// t_list	*parse_pipeline(t_node *pipeline)
// {
	
// }

// void	generate_cmd_tree(t_list *pipeline, t_node *node)
// {
// 	(void) pipeline;
// 	node->content.parenthesis = 'a';
// }

// void	populate_cmd_trees(t_node *node)
// {
// 	t_list	*pipeline;

// 	if (node == NULL)
// 		return ;
// 	populate_cmd_trees(node->left_child);
// 	if (node->type == PIPELINE)
// 	{
// 		// pipeline = parse_pipeline(node);
// 		// generate_cmd_tree(pipeline, node);
// 	}
// 	populate_cmd_trees(node->right_child);
// }

void	free_tree(t_node *tree)
{
	if (tree == NULL)
		return ;
	free_tree(tree->left_child);
	free_tree(tree->right_child);
	if (tree->type == PIPELINE)
		free(tree->content.str);
	free(tree);
}

// int	main(void)
// {
// 	t_content	content;
// 	t_list		*lst;
// 	t_node		*tree;

// 	// content.parenthesis = '(';
// 	// lst = ft_lstnew(node_new(content, PAR));
// 	// content.parenthesis = '1';
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(content, PIPELINE)));
// 	// content.logic = AND;
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(content, LOGIC)));
// 	// content.parenthesis = '2';
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(content, PIPELINE)));
// 	// content.parenthesis = ')';
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(content, PAR)));
// 	// content.logic = OR;
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(content, LOGIC)));
// 	// content.parenthesis = '3';
// 	// ft_lstadd_back(&lst, ft_lstnew(node_new(content, PIPELINE)));

// 	content.parenthesis = '1';
// 	lst = ft_lstnew(node_new(content, PIPELINE));
// 	content.logic = OR;
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, LOGIC)));
// 	content.parenthesis = '(';
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PAR)));
// 	content.parenthesis = '(';
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PAR)));
// 	content.parenthesis = '2';
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PIPELINE)));
// 	content.logic = AND;
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, LOGIC)));
// 	content.parenthesis = '3';
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PIPELINE)));
// 	content.parenthesis = ')';
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PAR)));
// 	content.logic = AND;
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, LOGIC)));
// 	content.parenthesis = '4';
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PIPELINE)));
// 	content.parenthesis = ')';
// 	ft_lstadd_back(&lst, ft_lstnew(node_new(content, PAR)));
// 	tree = create_logic_tree(lst);
// 	populate_cmd_trees(tree);
// 	free_tree(tree);
// 	return (0);
// }
