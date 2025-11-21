/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:56:55 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/21 17:18:44 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigexit_child(int signal)
{
	if (signal == SIGTSTP)
		ft_exit(0);
}

int	set_signal_child(void)
{
	struct sigaction	sigint;
	struct sigaction	sigexit;

	ft_bzero(&sigint, sizeof(struct sigaction));
	sigint.sa_handler = SIG_DFL;
	if (sigaction(SIGINT, &sigint, NULL) == -1)
		return (perror("sigaction"), 1);
	ft_bzero(&sigexit, sizeof(struct sigaction));
	sigexit.sa_handler = &sigexit_child;
	if (sigaction(SIGTSTP, &sigexit, NULL) == -1)
		return (perror("sigaction"), 1);
	return (0);
}

int	handle_termios_child(void)
{
	char			*tty;
	int				fd;
	struct termios	termios_p;

	tty = ttyname(0);
	ft_printf("%s\n", tty);
	fd = open(tty, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
		return (perror("open"), 1);
	if (!isatty(fd))
		return (perror("isatty"), 1);
	if (tcgetattr(fd, &termios_p) < 0)
		return (perror("tcgetattr"), 1);
	termios_p.c_cc[VSUSP] = 4;
	termios_p.c_cc[VQUIT] = 31;
	if (tcsetattr(fd, TCSANOW, &termios_p))
		return (perror("tcsetattr"), 1);
	close(fd);
	return (0);
}

int	handle_signals_child(void)
{
	if (handle_termios_child() == 1)
		return (1);
	if (set_signal_child() == 1)
		return (1);
	return (0);
}
