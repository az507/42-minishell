/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_execve.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:55:20 by achak             #+#    #+#             */
/*   Updated: 2024/04/17 20:52:30 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	wrapper(int func_rv, const char *func_name)
{
	if (func_rv == -1)
		perror(func_name);
}

int	check_if_cmd_is_builtin(char *cmd)
{
	if (!my_strncmp("echo", cmd, 4))
		return (1);
	else if (!my_strncmp("pwd", cmd, 3))
		return (1);
	else if (!my_strncmp("cd", cmd, 2))
		return (1);
	else if (!my_strncmp("export", cmd, 6))
		return (1);
	else if (!my_strncmp("unset", cmd, 5))
		return (1);
	else if (!my_strncmp("env", cmd, 3))
		return (1);
	else if (!my_strncmp("exit", cmd, 4))
		return (1);
	else
		return (0);
}

char	*check_if_fork_needed(t_params *params, char *line_read)
{
	char	*builtin;

	if (params->cmd_nbr > 1)
		return (NULL);
//	if (check_if_cmd_is_builtin(params->cmd_arr[0].cmd_args[0]))
//		return (0);
//	else
//		return (1);
	builtin = check_if_first_cmd_is_builtin(line_read, *(params->head_env));
	if (builtin)
		return (builtin);
	else
		return (NULL);
}

void	skip_redir_and_filename(char **line_read)
{
	int	flag;

	flag = 0;
	while (**line_read == '<' || **line_read == '>' || is_whitespace(**line_read))
		(*line_read)++;
	while (**line_read)
	{
		if ((flag == 1 && **line_read == 39)
			|| (flag == 2 && **line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			;
		else if (**line_read == 39)
			flag = 1;
		else if (**line_read == '"')
			flag = 2;
		else if (is_whitespace(**line_read) || **line_read == '|')
			break ;
		(*line_read)++;
	}
}

char	*determine_whats_first_word(char *line_read, t_env *head_env)
{
	int		len;
	int		rv;
	char	*word;

	len = count_token_len(line_read, head_env);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	word[len] = '\0';
	copy_token_from_line(&line_read, word, head_env);
	rv = check_if_cmd_is_builtin(word);
	if (rv == 0)
	{
		free(word);
		return (NULL);
	}
	return (word);
}

char	*check_if_first_cmd_is_builtin(char *line_read, t_env *head_env)
{
	int		flag;
	char	*temp;

	flag = 0;
	temp = line_read;
	while (*temp)
	{
//		if ((flag == 1 && *temp == 39)
//			|| (flag == 2 && *temp == '"'))
//			flag = 0;
//		else if (flag == 1 || flag == 2)
//			;
//		else if (*temp == 39)
//			flag = 1;
//		else if (*temp == '"')
//			flag = 2;
		if (*temp == '|')
			return (0);
		else if (*temp == '<' || *temp == '>')
		{
			skip_redir_and_filename(&temp);
			continue ;
		}
		else if (!is_whitespace(*temp))
			return (determine_whats_first_word(line_read, head_env));
		else if (is_whitespace(*temp))
			;
		temp++;
	}
	return (NULL);
}

void	fork_and_execve(t_params *params, int i, int *new_fds, int *old_fds)
{
	pid_t	pids;

//	while (++i < params->cmd_nbr)
//	{
	if (++i == params->cmd_nbr)
		return ;
//	printf("i = %d\n", i);
//	printf("params->cmd_nbr = %d\n", params->cmd_nbr);
	if (i + 1 != params->cmd_nbr)
		wrapper(pipe(new_fds), "pipe");
	wrapper(pids = fork(), "fork");
	if (pids == 0)
		child_process(params, i, new_fds, old_fds);
	else
	{
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
	//	wait(NULL);
		fork_and_execve(params, i, new_fds, old_fds);
	}
//			parent_process(params, i, new_fds, old_fds);
//	}
}

void	preparing_fork_and_execve(t_params *params, char *line_read)
{
	int		i;
	int		new_fds[2];
	int		*old_fds;
	int		j;
	int		wstatus;
	char	*exit_str;
	char	*str;

	i = -1;
	wstatus = 0;
	old_fds = NULL;
	j = 0;
	exit_str = NULL;
	//if (!check_if_fork_needed(params, line_read))
	str = check_if_fork_needed(params, line_read);
	if (str)
	{
		free(str);
		//printf("params->cmd_arr[0].raw_str in prepare fork fn: %s\n",
				//params->cmd_arr[0].raw_str);
		child_process(params, ++i, NULL, NULL);
		//execute_builtin(params, 0, 0, NULL);
	}
	else
	{
		if (params->cmd_nbr > 1)
		{
			old_fds = malloc(sizeof(int) * 2);
			if (!old_fds)
				return ;
			old_fds[0] = -1;
			old_fds[1] = -1;
		}
		fork_and_execve(params, i, new_fds, old_fds);
		if (old_fds)
			free(old_fds);
		while (j++ < (params->cmd_nbr - 1))
			wait(NULL);
		wait(&wstatus);
//		while (waitpid(-1, &wstatus, WNOHANG) > 0)
//			;
		if (WIFEXITED(wstatus))
		{
			exit_str = strjoin_and_free_str("?=",
					ft_itoa(WEXITSTATUS(wstatus)), 2);
			if (!check_if_entry_exists("?", *(params->head_env)))
				create_new_entry(exit_str, params->head_env);
			else
				update_existing_entry(exit_str, params->head_env);
			free(exit_str);
		}
		cleanup_params(params);
	}
}
