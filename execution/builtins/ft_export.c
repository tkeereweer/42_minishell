/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 12:16:15 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/04 11:33:25 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**get_envvar_pointer(char *var, char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(var, env[i], ft_strlen(var)) == 0
			&& env[i][ft_strlen(var)] == '=')
			return (&env[i]);
		i++;
	}
	return (NULL);
}

int	get_key_len(char *key_val)
{
	int	i;
	int	idx;

	i = 0;
	idx = -1;
	while (key_val[i] != '\0')
	{
		if (key_val[i] == '=')
		{
			idx = i;
			break ;
		}
		i++;
	}
	if (idx == 0)
		return (-1);
	else if (idx == -1)
		return (i);
	return (idx);
}

int	add_key(char *key_val, t_data *data)
{
	int		i;
	char	**temp;

	i = 0;
	while (data->env[i] != NULL)
		i++;
	temp = tab_realloc(data->env, i + 1);
	if (temp == NULL)
		return (1);
	data->env = temp;
	data->env[i] = ft_strdup(key_val);
	if (data->env[i] == NULL)
		return (1);
	data->env[i + 1] = NULL;
	return (0);
}

int	key_already_exists(char **curr_key_val, char *key_val)
{
	char	*tmp;

	tmp = *curr_key_val;
	*curr_key_val = ft_strdup(key_val);
	if (*curr_key_val == NULL)
		return (1);
	free(tmp);
	return (0);
}

char	**tab_cpy(char **tab)
{
	int		i;
	char	**tab_cpy;

	i = 0;
	while (tab[i] != NULL)
		i++;
	tab_cpy = (char **) malloc((i + 2) * sizeof(char *));
	if (tab_cpy == NULL)
		return (NULL);
	i = 0;
	while (tab[i] != NULL)
	{
		tab_cpy[i] = ft_strdup(tab[i]);
		if (tab_cpy[i] == NULL)
		{
			free_split(tab_cpy);
			return (NULL);
		}
		i++;
	}
	tab_cpy[i] = NULL;
	return (tab_cpy);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	i = 0;
	while (*str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
		i++;
	}
	return (*str1 - *str2);
}

void	sort_tab(char **tab)
{
	int		i;
	int		j;
	int		tab_len;
	char	*temp;

	i = 0;
	while (tab[i] != NULL)
		i++;
	tab_len = i;
	i = 0;
	while (i < tab_len)
	{
		j = 0;
		while (j < tab_len - i - 1)
		{
			if (ft_strcmp(tab[j], tab[j + 1]) > 0)
			{
				temp = tab[j];
				tab[j] = tab[j + 1];
				tab[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

int	print_sort_env(t_data *data)
{
	char	**env_cpy;
	int		i;

	env_cpy = tab_cpy(data->env);
	if (env_cpy == NULL)
		return (1);
	sort_tab(env_cpy);
	i = 0;
	while (env_cpy[i] != NULL)
	{
		if (!(env_cpy[i][0] == '_' && env_cpy[i][1] == '='))
			ft_printf("%s\n", env_cpy[i]);
		i++;
	}
	free_split(env_cpy);
	return (0);
}

int	ft_export(char *key_val, t_data *data)
{
	char	*key;
	int		key_len;
	char	**curr_key_val;

	if (key_val == NULL)
		return (print_sort_env(data));
	key_len = get_key_len(key_val);
	if (key_len == -1)
	{
		ft_printf("export: `%s': not a valid identifier\n", key_val);
		return (1);
	}
	key = ft_substr(key_val, 0, key_len);
	if (key == NULL)
		return (1);
	curr_key_val = get_envvar_pointer(key, data->env);
	free(key);
	if (curr_key_val != NULL)
		return (key_already_exists(curr_key_val, key_val));
	return (add_key(key_val, data));
}
