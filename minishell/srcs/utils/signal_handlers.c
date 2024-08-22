/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 13:30:21 by achak             #+#    #+#             */
/*   Updated: 2024/04/22 08:30:10 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_async_flag = 1;
		write(STDIN_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_up_signals(void)
{
	struct sigaction	act1;
	struct sigaction	act2;

	act1.sa_handler = signal_handler;
	act2.sa_handler = SIG_IGN;
	act1.sa_flags = 0;
	act2.sa_flags = 0;
	sigemptyset(&act1.sa_mask);
	sigemptyset(&act2.sa_mask);
	sigaction(SIGINT, &act1, NULL);
	sigaction(SIGQUIT, &act2, NULL);
}

/*signal(SIGQUIT, SIG_IGN);*/
void	non_interactive_signal_handler(void)
{
	struct sigaction	act;

	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
}

void	set_signals_to_dfl(void)
{
	struct sigaction	act1;
	struct sigaction	act2;

	act1.sa_handler = SIG_DFL;
	act1.sa_flags = 0;
	sigemptyset(&act1.sa_mask);
	act2.sa_handler = SIG_DFL;
	act2.sa_flags = 0;
	sigemptyset(&act2.sa_mask);
	sigaction(SIGINT, &act1, NULL);
	sigaction(SIGQUIT, &act2, NULL);
}
