/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 12:16:15 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 10:21:27 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	**tab_cpy(char **tab)
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

static void	sort_tab(char **tab)
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

static int	print_sort_env(t_data *data)
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

int	ft_export_var(char *key_val, t_data *data)
{
	char	*key;
	int		key_len;
	char	**curr_key_val;

	key_len = get_key_len(key_val);
	if (valid_key(key_val, key_len) == 0)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(key_val, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
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

int	ft_export(char **argv, t_data *data)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	if (argv[1] == NULL)
		return (print_sort_env(data));
	while (argv[i] != NULL)
	{
		if (ft_export_var(argv[i], data) == 1)
			ret = 1;
		i++;
	}
	return (ret);
}
