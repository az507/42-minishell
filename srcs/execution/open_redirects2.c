/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirects2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 13:40:53 by achak             #+#    #+#             */
/*   Updated: 2024/04/19 13:51:41 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_type_of_redirect(char *temp)
{
	if (*temp == '>')
	{
		if (*(temp + 1))
		{
			if (*(temp + 1) == '>')
				return (1);
		}
		return (2);
	}
	else if (*temp == '<')
		return (3);
	return (0);
}

void	close_existing_fds(t_params *params, int i, int open_mode)
{
	if ((open_mode == 1 || open_mode == 2)
		&& params->cmd_arr[i].stdout_fd != -2)
		wrapper(close(params->cmd_arr[i].stdout_fd), "close");
	else if (open_mode == 3 && params->cmd_arr[i].heredoc_rightmost == 0
		&& params->cmd_arr[i].stdin_fd != -2)
	{
		wrapper(close(params->cmd_arr[i].stdin_fd), "close");
		if (params->cmd_arr[i].heredoc)
		{
			wrapper(unlink(params->cmd_arr[i].heredoc), "unlink");
			free(params->cmd_arr[i].heredoc);
			params->cmd_arr[i].heredoc = NULL;
		}
	}
	else if (open_mode == 3 && params->cmd_arr[i].heredoc_rightmost == -1
		&& params->cmd_arr[i].stdin_fd != -2)
		wrapper(close(params->cmd_arr[i].stdin_fd), "close");
}

int	open_call_error_or_not(t_params *params, int i, char *filename)
{
	if (params->cmd_arr[i].stdin_fd == -1 || params->cmd_arr[i].stdout_fd == -1)
	{
		perror(filename);
		free(filename);
		return (-1);
	}
	return (0);
}

void	erase_redirs_from_str(char **temp, int *flag)
{
	while (**temp)
	{
		if ((*flag == 1 && **temp == 39)
			|| (*flag == 2 && **temp == '"'))
			*flag = 0;
		else if (*flag == 1 || *flag == 2)
			;
		else if (**temp == 39)
			*flag = 1;
		else if (**temp == '"')
			*flag = 2;
		else if (is_whitespace(**temp) || **temp == '<' || **temp == '>'
			|| **temp == '|')
		{
			*flag = 3;
			break ;
		}
		if (**temp == 39 || **temp == '"' || *flag == 1 || *flag == 2
			|| !is_whitespace(**temp))
			**temp = ' ';
		(*temp)++;
	}
}
