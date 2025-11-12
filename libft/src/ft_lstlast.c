/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:04:44 by mturgeon          #+#    #+#             */
/*   Updated: 2025/09/18 17:59:09 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next)
		temp = temp->next;
	return (temp);
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
	t_list *last = ft_lstlast(*list);
    printf("last elem: %s\n", (char *) last->content);
}*/