/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:21:39 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/08 17:57:09 by mturgeon         ###   ########.fr       */
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
	else if (type == REDIR)
		new->content.redir = content.redir;
	else
		new->content = content;
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

void	cut_at_eob(t_list **list)
{
	int	par_cnt;

	par_cnt = 1;
	*list = (*list)->next;
	while (par_cnt != 0 && list != NULL)
	{
		if ((*list)->content->type == PAR && (*list)->content->content.parenthesis == '(')
			par_cnt++;
		else if ((*list)->content->type == PAR && (*list)->content->content.parenthesis == ')')
			par_cnt--;
		*list = (*list)->next;
	}
	if (*list != NULL)
	{
		(*list)->prev->next = NULL;
		(*list)->prev = NULL;
	}
}

static void move_list(t_node **node, t_list **list)
{
	(*node)->right_child = (*list)->content;
	(*list) = (*list)->next;
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
			cut_at_eob(&list);
			node->right_child = populate_logic_tree(start_of_block);
		}
		else if (list->content->type == PIPELINE)
			move_list(&node, &list);
		node->right_child->parent = node;
	}
	return (node);
}

t_node	*create_logic_tree(t_list *list)
{
	t_node	*ret;
	t_list	*tmp;

	ret = populate_logic_tree(list);
	while (list != NULL)
	{
		tmp = list;
		list = list->next;
		if (tmp->content->type == PAR && tmp->content->content.parenthesis == '(')
			free(tmp->content);
		free(tmp);
	}
	return (ret);
}
// free malloced pointers in tree->content
void	free_tree(t_node *tree)
{
	if (tree == NULL)
		return ;
	free_tree(tree->left_child);
	free_tree(tree->right_child);
	if (tree->type == ARGS)
		free_split(tree->content.tab);
	else if (tree->type == REDIR)
		free(tree->content.redir.path);
	free(tree);
}
