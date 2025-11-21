/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:39:41 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/21 17:06:15 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

typedef struct s_make_tab
{
	int		word_count;
	int 	i;
	int 	j;
	char    *str;
	char	**tab;
}   t_make_tab;

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
	tab[1] = NULL;
	return (tab);
}

static char **make_tab(char **tab, char *temp, int *word_count)
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

static char **make_last_tab(char **tab, char *temp, int word_count)
{
	if (!temp)
		return (free_split(tab), NULL);
	tab = tab_realloc(tab, word_count + 1);
	if (!tab)
		return(free_split(tab), NULL);
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
t_make_tab	init_struct(char *str)
{
	t_make_tab	utils;

	utils.i = 0;
	utils.j = 0;
	utils.word_count = 0;
	utils.str = str;
	utils.tab = NULL;
	return (utils);
}
char	**args_tab(char *str)
{
	t_make_tab	s;
	char    *temp;

	s = init_struct(str);
	while(str[s.i])
	{
		if (!iterate_over_quotes(str, &(s.i)))
			return (NULL);
		if (ft_is_whitespace(str[s.i]))
		{
			temp = ft_substr(str, s.j, s.i - s.j);
			s.tab = make_tab(s.tab, temp, &s.word_count);
			if (!s.tab)
				return (NULL);
			while (ft_is_whitespace(str[s.i]))
				s.i++;
			s.j = s.i;
			continue;
		}
		s.i++;
	}
	if (!s.tab)
		return (tab_1_string(s.tab, str));
	temp = ft_substr(str, s.j, s.i - s.j);
	return (make_last_tab(s.tab, temp, s.word_count));
}
