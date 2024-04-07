/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locate_command_names.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:27:15 by achak             #+#    #+#             */
/*   Updated: 2024/04/07 13:48:43 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_for_char_in_str(char *str, char c)
{
	int	i;

	i = -1;
	if (str && *str)
	{
		while (str[++i])
		{
			if (str[i] == c)
				return (1);
		}
	}
	return (0);
}

int	check_and_assign_cmd_path2(t_params *params, int i)
{
	char	*cwd;

	cwd = strjoin_and_free_str(getcwd(NULL, 200), "/", 1);
	cwd = strjoin_and_free_str(cwd, params->cmd_arr[i].cmd_args[0], 1);
	if (access(cwd, F_OK | X_OK) == -1)
	{
		free(cwd);
//		ft_dprintf(STDERR_FILENO, "%s: %s\n",
//			params->cmd_arr[i].cmd_args[0], strerror(errno));
		perror(params->cmd_arr[i].cmd_args[0]);
		return (0);
	}
	free(cwd);
	return (1);
}

int	check_and_assign_cmd_path(t_params *params, int i, char **arr_path)
{
	int		j;
	char	*cmd_path;

	j = -1;
	cmd_path = NULL;
	if (arr_path)
	{
		while (arr_path[++j])
		{
			//arr_path[j] = strjoin_and_free_str(arr_path[j], "/", 1);
			cmd_path = strjoin_and_free_str(arr_path[j], "/", 0);
			//cmd_path = strjoin_and_free_str(arr_path[j], cmd_path, 2);
			cmd_path = strjoin_and_free_str(cmd_path,
				params->cmd_arr[i].cmd_args[0], 1);
			//printf("cmd_path = %s\n", cmd_path);
			if (access(cmd_path, F_OK | X_OK) == 0)
			{
				free(params->cmd_arr[i].cmd_args[0]);
				params->cmd_arr[i].cmd_args[0] = cmd_path;
				free_array(arr_path);
				return (1);
			}
			free(cmd_path);
		}
		free_array(arr_path);
	}
	return (check_and_assign_cmd_path2(params, i));
}

int	locate_command_names(t_params *params, int i)
{
	char	**arr_path;
	t_env	*temp;

	arr_path = NULL;
	temp = *(params->head_env);
	if (check_for_char_in_str(params->cmd_arr[i].cmd_args[0], '/'))
		return (0);
	if (check_if_cmd_is_builtin(params->cmd_arr[i].cmd_args[0]))
		return (0);
	while (temp)
	{
		if (temp->key)
			if (!my_strncmp("PATH", temp->key, 4))
				break ;
		temp = temp->next;
	}
	if (temp)
	{
		arr_path = ft_split(temp->value, ':');
		if (!*arr_path)
			free_array(arr_path);
	}
	else
	{
//		ft_dprintf(STDERR_FILENO, "%s: command not found\n",
//			params->cmd_arr[i].cmd_args[0]);
		if (access(params->cmd_arr[i].cmd_args[0], F_OK | X_OK) == -1)
			perror(params->cmd_arr[i].cmd_args[0]);
		return (-1);
	}
	if (!check_and_assign_cmd_path(params, i, arr_path))
		return (-1);
//	if (!check_and_assign_cmd_path(params,
//			params->cmd_arr[i].cmd_args[0], i))
//		return (-1);
	return (0);
}
