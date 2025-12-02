/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 10:50:02 by mkeerewe          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/12/02 11:36:25 by mkeerewe         ###   ########.fr       */
=======
/*   Updated: 2025/12/02 15:34:31 by mturgeon         ###   ########.fr       */
>>>>>>> fix_quotes_
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	clean_quotes(char *str, int i, char type)
{
<<<<<<< HEAD
    if (str[i + 1] == type)
=======
    if (str[i + 1] == type) //to handle the "" and '' cases
>>>>>>> fix_quotes_
    {
        str[i] = '\0';
        str[i + 1] = '\0';
        return (1);
    }
	ft_strncpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]));
	if (str[i + 1] != '\0')
		i++;
	while (str[i] != type)
		i++;
	ft_strncpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]));
	return (i - 1);
}

int	remove_quotes(char **str)
{
	int	i;

	i = 0;
	while ((*str)[i] != '\0')
	{
		if ((*str)[i] == '\'' || (*str)[i] == '"')
			i = clean_quotes(*str, i, (*str)[i]);
		i++;
	}
	*str = my_realloc(*str, ft_strlen(*str) + 1);
	if (*str == NULL)
		return (1);
	return (0);
}

int	expand_vars(char ***tab, t_data *data)
{
	int	i;

	i = 0;
	while ((*tab)[i] != NULL)
	{
		if (expand_envvars(&(*tab)[i], data) == -1)
			return (-1);
		if (expand_wildcards(tab, i, (*tab)[i]) == -1)
			return (-1);
		if (remove_quotes(&(*tab)[i]) == 1)
			return (-1);
		i++;
	}
	return (0);
}

int	expand_vars_redir(char **path, t_data *data)
{
    int res;
    //expand functions return -2 on ambig redirect
	res = expand_envvars_redir(path, data);
    if (res < 0)
		return (res);
	res = expand_wildcards_redir(path, *path);
    if (res < 0)
		return (res);
	if (remove_quotes(path) == 1)
		return (-1);
	return (0);
}
