/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 13:19:57 by achak             #+#    #+#             */
/*   Updated: 2024/04/16 12:33:34 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	child_process2(t_params *params, int i, int *new_fds, int *old_fds)
{
	char	**envp;

	envp = create_envp_arr(*(params->head_env));
	if (locate_command_names(params, i) == -1)
	{
		cleanup_params(params);
		free(old_fds);
		exit(EXIT_FAILURE);
	}
//	printf("at the end of childprocess fn\n");
//	printf("closing params stdin fd to see wat happens\n");
//	//wrapper(close(params->cmd_arr[i].stdin_fd), "close");
//	printf("ABCABCABCABCABC\n");
	settle_child_process_stdin(params, i, new_fds, old_fds);
	settle_child_process_stdout(params, i, new_fds, old_fds);
	if (!new_fds && !old_fds)
	{
		free_array(envp);
		//printf("in here\n");
		execute_builtin(params, i, 0, NULL);
	}
	else if (check_if_cmd_is_builtin(params->cmd_arr[i].cmd_args[0]))
	{
		free_array(envp);
		execute_builtin(params, i, 1, old_fds);
	}
	else if (params->cmd_arr[i].cmd_path)
	{
		//envp = create_envp_arr(*(params->head_env));
//		int	j = -1;
//		while (envp[++j])
//			printf("envp[%d] = %s\n", j, envp[j]);
		execve(params->cmd_arr[i].cmd_path,
			params->cmd_arr[i].cmd_args, envp);
		perror(params->cmd_arr[i].cmd_args[0]);
		cleanup_params(params);
		free_array(envp);
		free(old_fds);
		exit(EXIT_FAILURE);
	}
	else
	{
		//envp = create_envp_arr(*(params->head_env));
//		int	j = -1;
//		while (envp[++j])
//			printf("envp[%d] = %s\n", j, envp[j]);
		execve(params->cmd_arr[i].cmd_args[0],
			params->cmd_arr[i].cmd_args, envp);
		perror(params->cmd_arr[i].cmd_args[0]);
		cleanup_params(params);
		free_array(envp);
		free(old_fds);
		exit(EXIT_FAILURE);
	}
}

void	child_process(t_params *params, int i, int *new_fds, int *old_fds)
{
	char	**cmd_args;

//	printf("old_fds = %p\n", old_fds);
//	printf("new_fds = %p\n", new_fds);
	cmd_args = NULL;
	if (open_redirects(params, i) == -1)
	{
		cleanup_params(params);
		free(old_fds);
		if (!new_fds && !old_fds)
			return ;
		exit(EXIT_FAILURE);
	}
	if (check_empty_line(params->cmd_arr[i].raw_str) == -1)
	{
		if (!new_fds && !old_fds)
			return ;
		exit(EXIT_SUCCESS);
	}
	cmd_args = parse_input_string
		(params->cmd_arr[i].raw_str, *(params->head_env));
	if (!cmd_args)
	{
		cleanup_params(params);
		free(old_fds);
		if (new_fds || old_fds)
			exit(EXIT_FAILURE);
	}
	params->cmd_arr[i].cmd_args = cmd_args;
	child_process2(params, i, new_fds, old_fds);
}
