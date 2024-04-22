/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_execve.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:55:20 by achak             #+#    #+#             */
/*   Updated: 2024/04/22 15:55:03 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	wrapper(int func_rv, const char *func_name)
{
	if (func_rv == -1)
		perror(func_name);
}

void	fork_and_execve(t_params *params, int i, int *new_fds, int *old_fds)
{
	pid_t	pids;

	if (++i == params->cmd_nbr)
		return ;
	if (i + 1 != params->cmd_nbr)
		wrapper(pipe(new_fds), "pipe");
	wrapper(pids = fork(), "fork");
	if (pids == 0)
		child_process(params, i, new_fds, old_fds);
	else
	{
		params->pid_arr[i] = pids;
		if (i != 0)
		{
			close(old_fds[0]);
			close(old_fds[1]);
		}
		if (i != params->cmd_nbr - 1)
		{
			old_fds[0] = new_fds[0];
			old_fds[1] = new_fds[1];
		}
		fork_and_execve(params, i, new_fds, old_fds);
	}
}

void	set_exit_status(t_params *params, int wstatus, int mode)
{
	char	*exit_str;

	if (!mode)
		exit_str = strjoin_and_free_str("?=",
				ft_itoa(WEXITSTATUS(wstatus)), 2);
	else if (mode == 1)
		exit_str = strjoin_and_free_str("?=", ft_itoa((wstatus)), 2);
	else
		exit_str = strjoin_and_free_str("?=",
				ft_itoa(WTERMSIG(wstatus) + 128), 2);
	if (!check_if_entry_exists("?", *(params->head_env)))
		create_new_entry(exit_str, params->head_env);
	else
		update_existing_entry(exit_str, params->head_env);
	free(exit_str);
}

void	parent_waits_for_children(t_params *params, int *old_fds)
{
	char	*exit_str;
	int		wstatus;
	int		j;

	exit_str = NULL;
	j = 0;
	if (old_fds)
		free(old_fds);
	while (j < (params->cmd_nbr - 1))
		waitpid(params->pid_arr[j++], NULL, 0);
	waitpid(params->pid_arr[j], &wstatus, 0);
	free(params->pid_arr);
	params->pid_arr = NULL;
	if (WIFEXITED(wstatus))
		set_exit_status(params, wstatus, 0);
	else if (WIFSIGNALED(wstatus))
		set_exit_status(params, wstatus, 2);
	cleanup_params(params);
}

void	preparing_fork_and_execve(t_params *params, char *line_read)
{
	int		i;
	int		new_fds[2];
	int		*old_fds;

	i = -1;
	old_fds = NULL;
	non_interactive_signal_handler();
	if (check_if_fork_needed(params, line_read) == 0)
		set_exit_status(params, child_process(params, ++i, NULL, NULL), 1);
	else
	{
		params->pid_arr = malloc(sizeof(pid_t) * params->cmd_nbr);
		if (params->cmd_nbr > 1)
		{
			old_fds = malloc(sizeof(int) * 2);
			if (!old_fds || !params->pid_arr)
				return ;
			old_fds[0] = -1;
			old_fds[1] = -1;
		}
		fork_and_execve(params, i, new_fds, old_fds);
		parent_waits_for_children(params, old_fds);
	}
	set_up_signals();
}
