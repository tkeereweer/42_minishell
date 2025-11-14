/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 21:10:58 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/18 15:31:07 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	t_list	*temp;

	temp = new;
	new->next = *lst;
	*lst = temp;
	return ;
}
/*
int main(void)
{
	t_list *test0 = ft_lstnew("this is a test");
	t_list *test1 = ft_lstnew("2nd string");
	t_list *test2 = ft_lstnew("3rd one");

	t_list **list = &test0;
	ft_lstadd_front(list, test1);
	ft_lstadd_front(list, test2);
	t_list **temp = list;
	int i = 0;
	while ((*temp))
	{
		printf("element (%d): %s\n", i, (char *)(*temp)->content);
		*temp = (*temp)->next;
		i++;
	}
}*/