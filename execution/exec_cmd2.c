/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:59:37 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/26 13:58:06 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_path(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*temp1;

	i = 0;
	if (cmd[0] == '/')
		return (ft_strdup(cmd));
	temp1 = ft_strjoin("/", cmd);
	if (!temp1)
		return (NULL);
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], temp1);
		if (!temp)
			return (free(temp1), free_split(paths), NULL);
		if (access(temp, F_OK) == 0)
			return (free(temp1), free_split(paths), temp);
		free(temp);
		i++;
	}
	free_split(paths);
	return (NULL);
}

char	*get_exe_path(char **env, char *cmd)
{
	char	**paths;

	paths = ft_split(ft_getenv("$PATH", env), ':');
	if (!paths)
		return (NULL);
	return (find_path(paths, cmd));
}

void	cmd_not_found(char *cmd)
{
	ft_printf("minishell: command not found: %s\n", cmd);
	exit(127);
}

void	permission_error(char *path)
{
	ft_printf("minishell: %s: Permission denied\n", path);
	exit(126);
}

void	exec_fail(char *path, char *cmd)
{
	if (errno == EACCES || errno == EISDIR)
	{
		permission_error(path);
	}
	perror(cmd);
	exit(1);
}

// int setup_cmd(t_node *node, t_data *data, int mode)
// {
// 	char    *exec_path;
// 	// if (mode == 1)
// 	// {
// 	// 	return (run_builtins(node->content.tab, data));
// 	// }
// 	// else
// 	// {
// 		// exec_path = get_exe_path(data->env, node->content.tab[0]);
// 		// if (exec_path == NULL)
// 		// 	cmd_not_found(node->content.tab[0]);
// 		// if (execve(exec_path, node->content.tab, data->env) == -1)
// 		// 	exec_fail(exec_path, node->content.tab[0]);
// 	// }
// }
