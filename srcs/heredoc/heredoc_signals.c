/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:42:46 by achak             #+#    #+#             */
/*   Updated: 2024/04/21 18:14:40 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	heredoc_sighandler(int sig)
{
	if (sig == SIGINT)
		g_async_flag = 1;
}

void	heredoc_signal_handler(void)
{
	struct sigaction	act;

	act.sa_handler = heredoc_sighandler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
}

void	free_heredoc_stuff(char *str, char *heredoc, char *delim, int fd1)
{
	if (g_async_flag == 1)
	{
		if (str)
			free(str);
		if (heredoc)
		{
			unlink(heredoc);
			free(heredoc);
		}
		free(delim);
		close(fd1);
	}
}

int	cleanup_before_exiting_heredoc(t_params *params, int i)
{
	int	j;

	j = -1;
	while (++j < params->cmd_nbr)
		if (params->cmd_arr[i].stdin_fd != -2)
			close(params->cmd_arr[i].stdin_fd);
	update_existing_entry("?=130", params->head_env);
	cleanup_params(params);
	g_async_flag = 0;
	set_up_signals();
	return (-1);
}
