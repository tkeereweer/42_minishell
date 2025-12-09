/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mturgeon <maxime.p.turgeon@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:56:55 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/09 13:32:59 by mturgeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_signum;

void	sigint_parent(int signal)
{
	if (signal == SIGINT)
	{
		g_signum = SIGINT;
		printf("%c", '\n');
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigint_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		g_signum = SIGINT;
        close(STDIN_FILENO);
		printf("%c", '\n');
	}
}

int	set_signal_parent(int mode)
{
	struct sigaction	sigint;
	struct sigaction	sigquit;

	ft_bzero(&sigint, sizeof(struct sigaction));
	if (mode == 0)
		sigint.sa_handler = &sigint_parent;
	else if (mode == 1)
		sigint.sa_handler = SIG_IGN;
	else if (mode == 2)
		sigint.sa_handler = &sigint_heredoc;
	if (sigaction(SIGINT, &sigint, NULL) == -1)
		return (perror("sigaction"), 1);
	ft_bzero(&sigquit, sizeof(struct sigaction));
	sigquit.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sigquit, NULL) == -1)
		return (perror("sigaction"), 1);
	return (0);
}

int	handle_signals_parent(int mode)
{
	if (set_signal_parent(mode) == 1)
		return (1);
	return (0);
}
