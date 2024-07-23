/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_fns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 11:29:29 by achak             #+#    #+#             */
/*   Updated: 2024/04/22 15:55:23 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	free_symbol_table(t_env **head)
{
	t_env	*temp;

	while (*head)
	{
		temp = (*head)->next;
		free((*head)->key);
		free((*head)->value);
		free(*head);
		*head = temp;
	}
	*head = NULL;
}

void	free_array(char **arr)
{
	int	i;

	i = -1;
	if (arr && *arr)
		while (arr[++i])
			free(arr[i]);
	free(arr);
	arr = NULL;
}

void	free_cmd_arr(t_command *cmd_arr, int cmd_nbr)
{
	int	i;

	i = -1;
	while (++i < cmd_nbr)
	{
		if (cmd_arr[i].heredoc)
		{
			if (access(cmd_arr[i].heredoc, F_OK) == 0)
				unlink(cmd_arr[i].heredoc);
			free(cmd_arr[i].heredoc);
		}
		if (cmd_arr[i].cmd_args)
			free_array(cmd_arr[i].cmd_args);
		if (cmd_arr[i].cmd_path)
			free(cmd_arr[i].cmd_path);
		if (cmd_arr[i].raw_str)
			free(cmd_arr[i].raw_str);
	}
	free(cmd_arr);
	cmd_arr = NULL;
}

void	cleanup_params(t_params *params)
{
	if (params)
	{
		if (params->line_read)
			free(params->line_read);
		if (params->cmd_arr)
			free_cmd_arr(params->cmd_arr, params->cmd_nbr);
		if (params->pid_arr)
			free(params->pid_arr);
	}
}
