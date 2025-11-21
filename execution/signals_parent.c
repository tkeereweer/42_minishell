/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:56:55 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/21 17:16:38 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_parent(int signal)
{
	if (signal == SIGINT)
	{
		ft_printf("%c", '\n');
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sigexit_parent(int signal)
{
	if (signal == SIGTSTP)
		ft_exit(0);
}

int	set_signal_parent(void)
{
	struct sigaction	sigint;
	struct sigaction	sigexit;

	ft_bzero(&sigint, sizeof(struct sigaction));
	sigint.sa_handler = &sigint_parent;
	if (sigaction(SIGINT, &sigint, NULL) == -1)
		return (perror("sigaction"), 1);
	ft_bzero(&sigexit, sizeof(struct sigaction));
	sigexit.sa_handler = &sigexit_parent;
	if (sigaction(SIGTSTP, &sigexit, NULL) == -1)
		return (perror("sigaction"), 1);
	return (0);
}

int	handle_termios_parent(void)
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
	termios_p.c_cc[VQUIT] = 0;
	if (tcsetattr(fd, TCSANOW, &termios_p))
		return (perror("tcsetattr"), 1);
	close(fd);
	return (0);
}

int	handle_signals_parent(void)
{
	if (handle_termios_parent() == 1)
		return (1);
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
