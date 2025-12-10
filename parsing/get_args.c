/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:39:41 by mturgeon          #+#    #+#             */
/*   Updated: 2025/12/10 10:26:23 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**tab_1_string(char **tab, char *str)
{
	char	*temp;

	tab = tab_realloc(tab, 1);
	if (!tab)
		return (NULL);
	temp = ft_strdup(str);
	if (!temp)
		return (NULL);
	tab[0] = ft_strtrim(temp, " \t\n\v\f\r");
	if (!tab[0])
		return (free(temp), NULL);
	free(temp);
	free(str);
	tab[1] = NULL;
	return (tab);
}

static char	**make_tab(char **tab, char *temp, int *word_count)
{
	if (!temp)
		return (free_split(tab), NULL);
	*word_count += 1;
	tab = tab_realloc(tab, *word_count);
	if (!tab)
		return (free_split(tab), NULL);
	tab[*word_count - 1] = ft_strtrim(temp, " \t\n\v\f\r");
	if (!tab[*word_count - 1])
		return (free(temp), free_split(tab), NULL);
	free(temp);
	return (tab);
}

static char	**make_last_tab(char **tab, char *temp, int word_count)
{
	if (!temp)
		return (free_split(tab), NULL);
	tab = tab_realloc(tab, word_count + 1);
	if (!tab)
		return (free_split(tab), NULL);
	if (temp[0] == '\0')
	{
		free(temp);
		tab[word_count] = NULL;
		return (tab);
	}
	tab[word_count] = ft_strtrim(temp, " \t\n\v\f\r");
	if (!tab[word_count])
		return (free(temp), free_split(tab), NULL);
	free(temp);
	return (tab);
}

int	clean_args_nodes(t_list **head)
{
	t_list	*temp;
	char	**temp_tab;
	char	*temp_str;

	temp = *head;
	while (temp)
	{
		if (temp->content->type == ARGS)
		{
			temp_str = temp->content->content.str;
			temp_tab = args_tab(temp_str);
			if (!temp_tab)
				return (0);
			temp->content->content.tab = temp_tab;
		}
		temp = temp->next;
	}
	return (1);
}

char	**args_tab(char *str)
{
	t_make_tab	s;
	char		*temp;

	s = init_struct(str);
	while (str[s.i])
	{
		if (!iterate_over_quotes(str, &(s.i)))
			return (NULL);
		if (ft_is_whitespace(str[s.i]) || !str[s.i])
		{
			temp = ft_substr(str, s.j, s.i - s.j);
			s.tab = make_tab(s.tab, temp, &s.word_count);
			if (!s.tab)
				return (NULL);
			while (ft_is_whitespace(str[s.i]))
				s.i++;
			s.j = s.i;
			continue ;
		}
		s.i++;
	}
	if (!s.tab)
		return (tab_1_string(s.tab, str));
	temp = ft_substr(str, s.j, s.i - s.j);
	return (free(str), make_last_tab(s.tab, temp, s.word_count));
}
