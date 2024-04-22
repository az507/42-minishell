/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_funcs_for_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 18:48:08 by achak             #+#    #+#             */
/*   Updated: 2024/04/22 09:53:12 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	init_cmd_arr(t_params *params, t_command *cmd_arr, int cmd_nbr,
		char *line_read)
{
	int			i;
	t_command	*temp;

	i = -1;
	temp = cmd_arr;
	while (++i < cmd_nbr)
	{
		temp[i].cmd_args = NULL;
		temp[i].cmd_path = NULL;
		temp[i].raw_str = NULL;
		temp[i].heredoc = NULL;
		temp[i].stdin_fd = -2;
		temp[i].stdout_fd = -2;
		temp[i].heredoc_rightmost = -1;
	}
	params->cmd_arr = cmd_arr;
	params->cmd_nbr = cmd_nbr;
	params->line_read = line_read;
}

void	init_params(t_params *params, t_env **head_env)
{
	params->line_read = NULL;
	params->cmd_arr = NULL;
	params->pid_arr = NULL;
	params->cmd_nbr = 0;
	params->head_env = head_env;
}

void	check_read_line_eof(char *line_read, int flag, t_params *params)
{
	if (line_read)
		line_read = NULL;
	else if (flag)
	{
		free_symbol_table(params->head_env);
		rl_clear_history();
		write(STDOUT_FILENO, "exit\n", 5);
		exit(EXIT_SUCCESS);
	}
}
