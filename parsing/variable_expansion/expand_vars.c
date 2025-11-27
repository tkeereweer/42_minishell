/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 10:50:02 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/27 16:17:11 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	remove_quotes(char **str)
{
	char	*tmp;

    //remove balanced quotes when inside quotes of the same type
    //echo 'this is 'abc' test' --> this is abc test
    //echo 'this is "abc" test' --> this is "abc" test
    //echo "abc"thsi'test' --> abcthsitest
    //echo "abcthsi''test" --> abcthsi''test
	tmp = *str;
	*str = ft_substr(*str, 1, ft_strlen(*str) - 2);
	if (*str == NULL)
		return (1);
	free(tmp);
	return (0);
}

int	clean_quotes(char **str, int i, char type)
{
	ft_strncpy(&(*str)[i], &(*str)[i + 1], ft_strlen(&(*str)[i + 1]));
	i++;
	while ((*str)[i] != type)
		i++;
	ft_strncpy(&(*str)[i], &(*str)[i + 1], ft_strlen(&(*str)[i + 1]));
	return (i);
}

int	new_remove_quotes(char **str, char type)
{
	int	i;

	i = 0;
	while ((*str)[i] != '\0')
	{
		if ((*str)[i] == type)
			i = clean_quotes(str, i, type);
		i++;
	}
	*str = my_realloc(*str, ft_strlen(*str) + 1);
	if (*str == NULL)
		return (1);
	return (0);
}

int	*realloc_inttab(int *inttab, int new_size)
{
	int	*out;
	int i;

	out = (int *) malloc(new_size * sizeof(int));
	if (out == NULL)
		return (NULL);
	i = 0;
	while (inttab[i] != -1)
	{
		ft_memmove(out[i], inttab[i], sizeof(int));
		i++;
	}
	ft_memmove(out[i], inttab[i], sizeof(int));
	free(inttab);
	return (out);
}

new_expand_vars(char ***tab, t_data *data)
{
	int	i;
	int	j;
	int	k;
	int	*inttab;

	i = 0;
	while ((*tab)[i] != NULL)
	{
		k = 1;
		inttab = (int *) malloc(sizeof(int));
		if (inttab == NULL)
			return (-1);
		inttab[0] = -1;
		j = 0;
		while ((*tab)[i][j] == '\0')
		{
			if ((*tab)[i][j] == '"')
			{
				expand_envvars(&(*tab)[i][j], data, '"');
				while ((*tab)[i][j] != '"')
					j++;
			}
			j++;
		}
		new_remove_quotes((*tab)[i], '"');
		j = 0;
		while ((*tab)[i][j] == '\0')
		{
			if ((*tab)[i][j] == '\'')
			{
				while ((*tab)[i][j] != '\'')
				{
					if ((*tab)[i][j] == '*')
					{
						k++;
						inttab = realloc_inttab(inttab, k);
						if (inttab == NULL)
							return (-1);
						inttab[k - 1] = j - 1;
						inttab[k] = -1;
					}
					j++;
				}
			}
			j++;
		}
		new_remove_quotes((*tab)[i], '\'');
		expand_wildcards(tab, i, (*tab)[i], inttab);
	}
}
	
	





// 			if ((*tab)[i][j] == '\'')
// 			{
// 				while ((*tab)[i][j] != '\'')
// 					j++;
// 				j++;
// 			}
// 			else if ((*tab)[i][j] == '"')
// 			{
// 				expand_envvars(&(*tab)[i][j], data, '"');
// 				while ((*tab)[i][j] != '"')
// 					j++;
// 				j++;
// 			}
// 			else
// 			{
// 				expand_envvars(&(*tab)[i][j], data, '\0');
// 				new_remove_quotes((*tab)[i]);
// 				expand_wildcards(tab, i, &(*tab)[i][j]);
// 				while ((*tab)[i][j] != '\0' && (*tab)[i][j] != '\'' && (*tab)[i][j] != '"')
// 					j++;
// 			}
// 		}
// 		i++;
// 	}
// }

int	expand_vars(char ***tab, t_data *data)
{
	int	i;
	int	mode;

	i = 1;
	while ((*tab)[i] != NULL)
	{
		mode = 0;
		if ((*tab)[i][0] == '\'')
			mode = 2;
		else if ((*tab)[i][0] == '"')
			mode = 1;
		if (mode == 1 || mode == 2)
		{
			if (remove_quotes(&(*tab)[i]) == 1)
				return (1);
		}
		if (mode != 2)
		{
			if (expand_envvars(&(*tab)[i], data) == 1)
				return (1);
		}
		if (mode != 1)
		{
			if (expand_wildcards(tab, i, (*tab)[i]) == 1)
				return (1);
		}
		i++;
	}
	return (0);
}

int	expand_vars_redir(char **path, t_data *data)
{
	int	i;
	int	mode;

    
	i = 1;
	mode = 0;
	if (*path[0] == '\'')
		mode = 2;
	else if (*path[0] == '"')
		mode = 1;
	if (mode == 1 || mode == 2)
	{
		if (remove_quotes(path) == 1)
			return (-1);
	}
	if (mode != 2)
	{
		if (expand_envvars(path, data) == 1)
			return (-1);
	}
	if (mode != 1)
	{
		if (expand_wildcards_redir(path, *path) == 1)
			return (-1);
	}
	i++;
	return (1);
}
