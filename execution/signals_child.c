/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:56:55 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/12/10 10:28:05 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	signal_handler_child(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
	{
		if (signal == SIGQUIT)
			ft_printf("Quit (core dumped)");
		exit(signal);
	}
}

static int	set_signal_child(void)
{
	struct sigaction	sigint;
	struct sigaction	sigquit;

	ft_bzero(&sigint, sizeof(struct sigaction));
	sigint.sa_handler = &signal_handler_child;
	if (sigaction(SIGINT, &sigint, NULL) == -1)
		return (perror("sigaction"), 1);
	ft_bzero(&sigquit, sizeof(struct sigaction));
	sigquit.sa_handler = &signal_handler_child;
	if (sigaction(SIGQUIT, &sigquit, NULL) == -1)
		return (perror("sigaction"), 1);
	return (0);
}

int	handle_signals_child(void)
{
	if (set_signal_child() == 1)
		return (1);
	return (0);
}
