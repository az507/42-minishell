/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settle_child_process_stdin_and_out.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 16:23:54 by achak             #+#    #+#             */
/*   Updated: 2024/04/17 17:50:00 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	settle_child_process_stdin2(t_params *params, int i)
{
	if ((params->cmd_arr[i].stdin_fd) != -2)
	{
		wrapper(dup2((params->cmd_arr[i].stdin_fd),
				STDIN_FILENO), "dup2");
		wrapper(close((params->cmd_arr[i].stdin_fd)), "close");
		if (params->cmd_arr[i].heredoc)
		{
			wrapper(unlink(params->cmd_arr[i].heredoc), "unlink");
			free(params->cmd_arr[i].heredoc);
			params->cmd_arr[i].heredoc = NULL;
		}
	}
}

void	settle_child_process_stdin3(t_params *params, int i, int *old_fd)
{
	wrapper(close(old_fd[0]), "close");
	wrapper(dup2((params->cmd_arr[i].stdin_fd),
			STDIN_FILENO), "dup2");
	wrapper(close((params->cmd_arr[i].stdin_fd)), "close");
	if (params->cmd_arr[i].heredoc)
	{
		wrapper(unlink(params->cmd_arr[i].heredoc), "unlink");
		free(params->cmd_arr[i].heredoc);
		params->cmd_arr[i].heredoc = NULL;
	}
}

void	settle_child_process_stdin(t_params *params, int i, int *new_fd,
		int *old_fd)
{
	if (!new_fd && !old_fd && i == 0)
		return ;
	if (i == 0)
		settle_child_process_stdin2(params, i);
	else if (old_fd)
	{
		wrapper(close(old_fd[1]), "close");
		if ((params->cmd_arr[i].stdin_fd) != -2)
			settle_child_process_stdin3(params, i, old_fd);
		else
		{
			wrapper(dup2(old_fd[0], STDIN_FILENO), "dup2");
			wrapper(close(old_fd[0]), "close");
		}
	}
}

void	settle_child_process_stdout2(t_params *params, int i, int *new_fd)
{
	wrapper(close(new_fd[0]), "close");
	if ((params->cmd_arr[i].stdout_fd) != -2)
	{
		wrapper(close(new_fd[1]), "close");
		wrapper(dup2((params->cmd_arr[i].stdout_fd),
				STDOUT_FILENO), "dup2");
		wrapper(close((params->cmd_arr[i].stdout_fd)), "close");
	}
	else
	{
		wrapper(dup2(new_fd[1], STDOUT_FILENO), "dup2");
		wrapper(close(new_fd[1]), "close");
	}
}

void	settle_child_process_stdout(t_params *params, int i, int *new_fd,
		int *old_fd)
{
	if (!new_fd && !old_fd && i == 0)
		return ;
	if (i == params->cmd_nbr - 1)
	{
		if ((params->cmd_arr[i].stdout_fd) != -2)
		{
			wrapper(dup2((params->cmd_arr[i].stdout_fd),
					STDOUT_FILENO), "dup2");
			wrapper(close((params->cmd_arr[i].stdout_fd)), "close");
		}
	}
	else
		settle_child_process_stdout2(params, i, new_fd);
}
