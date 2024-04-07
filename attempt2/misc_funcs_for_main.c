/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_funcs_for_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 18:48:08 by achak             #+#    #+#             */
/*   Updated: 2024/04/07 15:15:24 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		write(STDIN_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	set_up_signals(void)
{
	struct sigaction	act;

	act.sa_handler = signal_handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	init_cmd_arr(t_command *cmd_arr, int cmd_nbr)
{
	int		i;
	t_command	*temp;

	i = -1;
	temp = cmd_arr;
	while (++i < cmd_nbr)
	{
		temp[i].cmd_args = NULL;
		temp[i].raw_str = NULL;
		temp[i].heredoc = NULL;
		temp[i].stdin_fd = -2;
		temp[i].stdout_fd = -2;
		temp[i].heredoc_rightmost = -1;
	}
}

void	init_params(t_params *params, t_env **head_env)
{
	params->cmd_arr = NULL;
	params->cmd_nbr = 0;
	params->head_env = head_env;
}

void	check_read_line_eof(char *line_read, int flag, t_params *params)
{
	if (line_read)
	{
		free(line_read);
		line_read = NULL;
	}
	else if (flag)
	{
		free_symbol_table(params->head_env);
		exit(EXIT_SUCCESS);
	}
}