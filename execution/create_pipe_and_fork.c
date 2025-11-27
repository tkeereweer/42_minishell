/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pipe_and_fork.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:48:21 by mturgeon          #+#    #+#             */
/*   Updated: 2025/11/26 17:28:52 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//initializes pipe tab
static int new_pipe_tab(t_data *data)
{
	int **temp;

	temp = int_tab_realloc(data->pipe_tab, 1);
	if (!temp)
		return (-1);
	data->pipe_tab = temp;
	if (pipe(data->pipe_tab[0]) == -1)
		return (-2);//pipe error
	return (1);
}

//create pipe, increment cmd_cnt and realloc pipe_tab
//pipe_cnt == cmd_cnt - 1
//returns 1 on success, <0 on error
//mode == 3 means last cmd hence no new pipe
int	create_pipe(t_data *data, int mode)
{
	int **temp;

	if (mode > 2)
		return (1);
	if (!data->pipe_tab)
		return (new_pipe_tab(data));//should we decrement cmd_cnt if pipe creation error ?
	temp = int_tab_realloc(data->pipe_tab, data->cmd_cnt);
	if (!temp)
		return (-1);
	data->pipe_tab = temp;
	if (pipe(data->pipe_tab[data->cmd_cnt - 1]) == -1)
		return (-2);//pipe error
	return (1);
}

int	is_builtin(char *name)
{
	char	*builtins[7];
	int		i;

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	i = 0;
	while (i < 7)
	{
		if (ft_strncmp(name, builtins[i], ft_strlen(name)) == 0)
			return (1);
		i++;
	}
	return (0);
}

//old_len of tab is size - 1, aka cmd_cnt - 1
static pid_t	*pid_arr_realloc(pid_t *arr, int size)
{
	pid_t	*dst;

	if (!arr)
		dst = (pid_t *)ft_calloc(1, sizeof(pid_t));
	else
		dst = (pid_t *)ft_calloc(size, sizeof(pid_t));
	if (!dst)
		return (NULL);
	if (!arr)
		return (dst);
	ft_memmove(dst, arr, (size - 1));
	free(arr);
	return (dst);
}

//close unused pipes here bc it will depend if child or not child
//if child, close after forking
int	create_fork(t_node *arg, t_data *data)
{
	pid_t	*temp;

	if (arg->type != ARGS)
		return (-3);//tree build error ?
	temp = pid_arr_realloc(data->pid_tab, data->cmd_cnt);
	if (!temp)
		return (-1);
	data->pid_tab = temp;
	return (1);
}
