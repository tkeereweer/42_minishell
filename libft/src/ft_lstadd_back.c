/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:40:30 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/12 12:25:54 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	temp = ft_lstlast(*lst);
	temp->next = new;
    new->prev = temp;
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
	int size = ft_lstsize(*list);
	t_list *temp = *list;
	int i = 0;
	while (temp)
	{
		printf("element (%d): %s\n", i, (char *)temp->content);
		temp = temp->next;
		i++;
	}
	printf("size = %d\n", size);
    t_list *last = ft_lstnew("blablabla");
    ft_lstadd_back(list, last);
    printf("last elem: %s\n", (char *) (ft_lstlast(*list))->content);
}*/