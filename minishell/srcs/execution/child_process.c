/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 13:19:57 by achak             #+#    #+#             */
/*   Updated: 2024/07/01 15:19:17 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	cleanup_in_child_process(t_params *params, char **envp, int *new_fds,
		int *old_fds)
{
	if (envp)
		free_array(envp);
	if (new_fds || old_fds)
	{
		rl_clear_history();
		free_symbol_table(params->head_env);
		cleanup_params(params);
		free(old_fds);
		return ;
	}
	cleanup_params(params);
}
/*
//	else if (params->cmd_arr[i].cmd_path)
//	{
//		envp = create_envp_arr(*(params->head_env));
//		execve(params->cmd_arr[i].cmd_path,
//			params->cmd_arr[i].cmd_args, envp);
//		perror(params->cmd_arr[i].cmd_args[0]);
//		cleanup_in_child_process(params, envp, new_fds, old_fds);
//		exit(EXIT_FAILURE);
//	}
//	else
//	{
//		envp = create_envp_arr(*(params->head_env));
//		execve(params->cmd_arr[i].cmd_args[0],
//		nalsrams->cmd_arr[i].cmd_args, envp);
//		perror(params->cmd_arr[i].cmd_args[0]);
//		cleanup_in_child_process(params, envp, new_fds, old_fds);
//		exit(EXIT_FAILURE);
//	}
*/

void	call_execve(t_params *params, int i, int *new_fds, int *old_fds)
{
	char	**envp;

	envp = create_envp_arr(*(params->head_env));
	if (params->cmd_arr[i].cmd_path)
		execve(params->cmd_arr[i].cmd_path,
			params->cmd_arr[i].cmd_args, envp);
	else
		execve(params->cmd_arr[i].cmd_args[0],
			params->cmd_arr[i].cmd_args, envp);
	if (g_async_flag == 1)
	{
		cleanup_in_child_process(params, envp, new_fds, old_fds);
		exit(130);
	}
	perror(params->cmd_arr[i].cmd_args[0]);
	cleanup_in_child_process(params, envp, new_fds, old_fds);
	exit(EXIT_FAILURE);
}

int	child_process2(t_params *params, int i, int *new_fds, int *old_fds)
{
	if (locate_command_names(params, i) == -1)
	{
		cleanup_in_child_process(params, NULL, new_fds, old_fds);
		exit(127);
	}
	settle_child_process_stdin(params, i, new_fds, old_fds);
	settle_child_process_stdout(params, i, new_fds, old_fds);
	if (!new_fds && !old_fds)
		return (execute_builtin(params, i, 0, NULL));
	else if (check_if_cmd_is_builtin(params->cmd_arr[i].cmd_args[0]))
		execute_builtin(params, i, 1, old_fds);
	else
		call_execve(params, i, new_fds, old_fds);
	return (0);
}

int	child_process1(t_params *params, int i, int *new_fds, int *old_fds)
{
	if (open_redirects(params, i) == -1)
	{
		cleanup_in_child_process(params, NULL, new_fds, old_fds);
		if (!new_fds && !old_fds)
			return (-1);
		exit(EXIT_FAILURE);
	}
	if (check_empty_line(params->cmd_arr[i].raw_str) == -1)
	{
		cleanup_in_child_process(params, NULL, new_fds, old_fds);
		if (!new_fds && !old_fds)
			return (-2);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

void	child_process_signal_handler(int sig)
{
	//if (sig == SIGINT || sig == SIGQUIT)
	if (sig == SIGINT)
		g_async_flag = 1;
}

void	set_signals_for_child_process(void)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = &child_process_signal_handler;
	if (sigaction(SIGINT, &act, NULL) == -1)
		perror("sigaction");
//	if (sigaction(SIGQUIT, &act, NULL) == -1)
//		perror("sigaction");
}

int	child_process(t_params *params, int i, int *new_fds, int *old_fds)
{
	char	**cmd_args;
	int		rv;

	if (old_fds || new_fds)
		set_signals_for_child_process();
		//set_signals_to_dfl();
	cmd_args = NULL;
	rv = child_process1(params, i, new_fds, old_fds);
	if (rv == -1)
		return (1);
	else if (rv == -2)
		return (0);
	cmd_args = parse_input_string
		(params->cmd_arr[i].raw_str, *(params->head_env));
	if (!cmd_args)
	{
		cleanup_in_child_process(params, NULL, new_fds, old_fds);
		if (!new_fds && !old_fds)
			return (1);
		exit(EXIT_FAILURE);
	}
	params->cmd_arr[i].cmd_args = cmd_args;
	return (child_process2(params, i, new_fds, old_fds));
}
