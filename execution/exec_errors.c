/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:59:37 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 12:37:47 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	redir_error(char *path, int mode)
{
	write(STDERR_FILENO, "minishell: ", 11);
	perror(path);
	if (mode == 4)
		return (1);
	exit(1);
}

int	ambig_redirect(void)
{
	write(STDERR_FILENO, "minishell: ", ft_strlen("minishell: "));
	write(STDERR_FILENO, "ambiguous redirect\n",
		ft_strlen(" ambiguous redirect\n"));
	return (-4);
}

void	cmd_not_found(char *cmd, t_data *data)
{
	ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	free_tree(data->tree);
	free_split(data->env);
	if (data->default_path != NULL)
		free(data->default_path);
	rl_clear_history();
	clean_data(data);
	exit(127);
}

void	permission_error(char *path, t_data *data)
{
	ft_putstr_fd("minishell:", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	free_tree(data->tree);
	free_split(data->env);
	if (data->default_path != NULL)
		free(data->default_path);
	rl_clear_history();
	free(path);
	clean_data(data);
	exit(126);
}

void	exec_fail(char *path, char *cmd, t_data *data)
{
	if (errno == EISDIR)
		perror(path);
	else if (errno == EACCES)
	{
		permission_error(path, data);
	}
	else
		perror(cmd);
	free_tree(data->tree);
	free_split(data->env);
	if (data->default_path != NULL)
		free(data->default_path);
	rl_clear_history();
	free(path);
	clean_data(data);
	if (errno == EISDIR)
		exit (126);
	if (errno == ENOENT)
		exit (127);
	exit(1);
}
