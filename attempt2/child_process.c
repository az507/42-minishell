/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 13:19:57 by achak             #+#    #+#             */
/*   Updated: 2024/04/07 17:55:43 by achak            ###   ########.fr       */
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

int	count_valid_var(t_env *temp_env, int *word_len)
{
	if (temp_env == NULL)
		return (0);
	*word_len = my_strlen(temp_env->value);
	return (1);
}

void	count_var_len_redir(t_params *params, char **temp, int *word_len)
{
	int	j;

	j = 0;
	(*temp)++;
	if (**temp)
	{
		if (!(is_alphabet(**temp) || **temp == '_'))
		{
			(*word_len)++;
			return ;
		}
		while ((*temp)[j] && (is_alphabet((*temp)[j]) || (*temp)[j] == '?'
			|| is_numeric((*temp)[j]) || (*temp)[j] == '_'))
		{
			if (j != 0 && (*temp)[j] == '?')
				break ;
			j++;
		}
		if (!count_valid_var
			(is_valid_var(*(params->head_env), *temp, j), word_len))
			return ;
		*temp += j;
	}
	else
		(*word_len)++;
}

int	count_word_len2(char *temp, int *word_len, int *flag)
{
	if (*flag == 1 || *flag == 2)
		(*word_len)++;
	else if (*temp == 39)
		*flag = 1;
	else if (*temp == '"')
		*flag = 2;
	else if (!is_whitespace(*temp))
		(*word_len)++;
	else if (is_whitespace(*temp))
		return (0);
	return (1);
}

int	count_word_len(t_params *params, char *temp)
{
	int	word_len;
	int	flag;

	word_len = 0;
	flag = 0;
	while (*temp && (*temp == '<' || *temp == '>' || is_whitespace(*temp)))
		temp++;
	while (*temp)
	{
		if ((flag == 1 && *temp == 39)
			|| (flag == 2 && *temp == '"'))
			flag = 0;
		else if (*temp == '$' && flag != 1)
		{
			count_var_len_redir(params, &temp, &word_len);
			continue ;
		}
		else if (count_word_len2(temp, &word_len, &flag) == 0)
			break ;
		temp++;
	}
	return (word_len);
}

t_env	*is_valid_var(t_env *temp_env, char *temp, int j)
{
	while (temp_env)
	{
		if (!my_strncmp(temp, temp_env->key, j))
			break ;
		temp_env = temp_env->next;
	}
	if (!temp_env)
		return (NULL);
	if (!temp_env->value)
		return (NULL);
	return (temp_env);
}

int	copy_valid_var(t_env *temp_env, char *filename, int *k)
{
	int	i;

	i = 0;
	if (temp_env == NULL)
		return (0);
	if (temp_env->value)
		while (i < my_strlen(temp_env->value))
			filename[(*k)++] = temp_env->value[i++];
	return (1);
}

void	copy_var_len_redir(t_params *params, char **temp, int *k,
		char *filename)
{
	int	j;

	j = 0;
	if (*++(*temp))
	{
		if (!(is_alphabet(**temp) || **temp == '_' || **temp == '?'))
		{
			filename[(*k)++] = '$';
			return ;
		}
		while ((*temp)[j] && (is_alphabet((*temp)[j]) || (*temp)[j] == '?'
			|| is_numeric((*temp)[j]) || (*temp)[j] == '_'))
		{
			if (j != 0 && (*temp)[j] == '?')
				break ;
			j++;
		}
		if (!copy_valid_var
			(is_valid_var(*(params->head_env), *temp, j), filename, k))
			return ;
		*temp += j;
	}
	else
		filename[(*k)++] = '$';
}

int	copy_str_to_filename2(char *filename, char *temp, int *flag, int *k)
{
	if (*flag == 1 || *flag == 2)
		filename[(*k)++] = *temp;
	else if (*temp == 39)
		*flag = 1;
	else if (*temp == '"')
		*flag = 2;
	else if (!is_whitespace(*temp))
		filename[(*k)++] = *temp;
	else if (is_whitespace(*temp))
		return (0);
	return (1);
}

void	copy_str_to_filename(t_params *params, char *filename, char *temp)
{
	int	flag;
	int	k;

	flag = 0;
	k = 0;
	while (*temp && (*temp == '<' || *temp == '>' || is_whitespace(*temp)))
		temp++;
	while (*temp)
	{
		if ((flag == 1 && *temp == 39)
			|| (flag == 2 && *temp == '"'))
			flag = 0;
		else if (*temp == '$' && flag != 1)
		{
			copy_var_len_redir(params, &temp, &k, filename);
			continue ;
		}
		else if (copy_str_to_filename2(filename, temp, &flag, &k) == 0)
			break ;
		temp++;
	}
}

char	*expand_filename(t_params *params, char *temp)
{
	int		file_len;
	char	*filename;

	file_len = count_word_len(params, temp);
	if (file_len == 0)
		return (NULL);
	filename = malloc(sizeof(char) * (file_len + 1));
	if (!filename)
		return (NULL);
	filename[file_len] = '\0';
	copy_str_to_filename(params, filename, temp);
	return (filename);
}

void	close_existing_fds(t_params *params, int i, int open_mode)
{
	if ((open_mode == 1 || open_mode == 2)
		&& params->cmd_arr[i].stdout_fd != -2)
		close(params->cmd_arr[i].stdout_fd);
	else if (open_mode == 3 && params->cmd_arr[i].heredoc_rightmost == 0)
	{
		close(params->cmd_arr[i].stdin_fd);
		if (params->cmd_arr[i].heredoc)
		{
			unlink(params->cmd_arr[i].heredoc);
			free(params->cmd_arr[i].heredoc);
			params->cmd_arr[i].heredoc = NULL;
		}
	}
	else if (open_mode == 3 && params->cmd_arr[i].heredoc_rightmost == -1
		&& params->cmd_arr[i].stdin_fd != -2)
		close(params->cmd_arr[i].stdin_fd);
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

int	open_new_files(t_params *params, int i, int open_mode, char *filename)
{
	int	fd;

	if (open_mode == 1)
		params->cmd_arr[i].stdout_fd
			= open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (open_mode == 2)
		params->cmd_arr[i].stdout_fd
			= open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (open_mode == 3)
	{
		fd = open(filename, O_RDONLY);
		if (fd == -1)
		{
			perror(filename);
			free(filename);
			return (-1);
		}
		if (params->cmd_arr[i].heredoc_rightmost == 1)
			close(fd);
		else
			params->cmd_arr[i].stdin_fd = fd;
	}
	return (open_call_error_or_not(params, i, filename));
}

void	erase_heredoc_and_delim(char **temp, int *flag)
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
		else if (is_whitespace(**temp))
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

int	perror_filename(char *filename, int open_mode)
{
	if (open_mode == 1)
		filename = "<<";
	else if (open_mode == 2)
		filename = "<";
	else if (open_mode == 3)
		filename = ">";
	access("", F_OK | X_OK);
	perror("''");
	return (-1);
}

int	settle_redirects(t_params *params, int i, char **temp)
{
	int		open_mode;
	int		fd;
	int		flag;
	char	*filename;

	open_mode = check_type_of_redirect(*temp);
	fd = -1;
	flag = 0;
	filename = expand_filename(params, *temp);
	if (!filename)
		return (perror_filename(filename, open_mode));
	close_existing_fds(params, i, open_mode);
	if (open_new_files(params, i, open_mode, filename) == -1)
		return (-1);
	while (**temp && (**temp == '<' || **temp == '>' || is_whitespace(**temp)))
	{
		**temp = ' ';
		(*temp)++;
	}
	erase_heredoc_and_delim(temp, &flag);
	free(filename);
	if (flag != 3)
		return (-2);
	return (0);
}

void	skip_quoted_chars(char *temp, int *flag)
{
	if ((*flag == 1 && *temp == 39)
		|| (*flag == 2 && *temp == '"'))
		*flag = 0;
	else if (*flag == 1 || *flag == 2)
		;
	else if (*temp == 39)
		*flag = 1;
	else if (*temp == '"')
		*flag = 2;
}

int	open_redirects(t_params *params, int i)
{
	int		flag;
	int		rv;
	char	*temp;

	flag = 0;
	rv = 0;
	temp = params->cmd_arr[i].raw_str;
	while (*temp)
	{
		if (flag != 0 || *temp == 39 || *temp == '"')
			skip_quoted_chars(temp, &flag);
		else if (*temp == '<' || *temp == '>')
		{
			rv = settle_redirects(params, i, &temp);
			if (rv == -1 || rv == -2)
				return (rv);
			continue ;
		}
		temp++;
	}
	return (0);
}

void	child_process2(t_params *params, int i, int *new_fds, int *old_fds)
{
	if (locate_command_names(params, i) == -1)
	{
		cleanup_params(params);
		free(old_fds);
		exit(EXIT_FAILURE);
	}
	settle_child_process_stdin(params, i, old_fds);
	settle_child_process_stdout(params, i, new_fds);
	if (!new_fds && !old_fds)
		execute_builtin(params, i, 0, NULL);
	else if (check_if_cmd_is_builtin(params->cmd_arr[i].cmd_args[0]))
		execute_builtin(params, i, 1, old_fds);
	else
	{
		execve(params->cmd_arr[i].cmd_args[0],
			params->cmd_arr[i].cmd_args, NULL);
		cleanup_params(params);
		free(old_fds);
		exit(EXIT_FAILURE);
	}
}

void	child_process(t_params *params, int i, int *new_fds, int *old_fds)
{
	char	**cmd_args;

	cmd_args = NULL;
	if (open_redirects(params, i) == -1)
	{
		cleanup_params(params);
		free(old_fds);
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
		exit(EXIT_FAILURE);
	}
	params->cmd_arr[i].cmd_args = cmd_args;
	child_process2(params, i, new_fds, old_fds);
}
