/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:56:55 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/25 10:18:15 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_signum; // check volatile

void	sigint_parent(int signal)
{
	if (signal == SIGINT)
	{
		g_signum = SIGINT;
		ft_printf("%c", '\n');
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	set_signal_parent(void)
{
	struct sigaction	sigint;
	struct sigaction	sigquit;

	ft_bzero(&sigint, sizeof(struct sigaction));
	sigint.sa_handler = &sigint_parent;
	if (sigaction(SIGINT, &sigint, NULL) == -1)
		return (perror("sigaction"), 1);
	ft_bzero(&sigquit, sizeof(struct sigaction));
	sigquit.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sigquit, NULL) == -1)
		return (perror("sigaction"), 1);
	return (0);
}

int	handle_signals_parent(void)
{
	if (set_signal_parent() == 1)
		return (1);
	return (0);
}

// int	main(void)
// {
// 	handle_termios();
// 	set_signal_parent();
// 	while (1)
// 		continue;
// 	return (0);
// }
