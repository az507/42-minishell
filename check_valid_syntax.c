/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_valid_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 10:05:25 by achak             #+#    #+#             */
/*   Updated: 2024/04/05 16:04:33 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_after_meta_char(char **line_read)
{
	if (*(*line_read + 1))
	{
		if (*(*line_read + 1) == '<' || *(*line_read + 1) == '>')
		{
			if (**line_read != *(*line_read + 1))
				return (-1);
		}
		else
		{
			while (**line_read && is_whitespace(**line_read))
				(*line_read)++;
			if (**line_read == '<' || **line_read == '>'
				|| **line_read == '|')
				return (-1);
			return (0);
		}
	}
	return (-1);
}

int	check_redir_syntax(char *line_read)
{
	int	flag;

	flag = 0;
	while (*line_read)
	{
		if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else* if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (*line_read == '<' || *line_read == '>')
		{
			if (check_after_meta_char(&line_read) == -1)
			{
				ft_dprintf(STDERR_FILENO,
					"syntax error near unexpected token `%c'",
						**line_read);
				return (-1);
			}
			continue ;
		}
		line_read++;
	}
	return (0);
}

int	check_syntax_around_pipe(char **line_read)
{
	if (*(*line_read + 1))
	{
		while (**line_read && is_whitespace(**line_read))
			(*line_read)++;
		if (**line_read)
		{
			if (**line_read == '|')
				return (-1);
			return (0);
		}
		else
			return (-1);
	}
	return (-1);
}

int	nbr_of_pipes(char *line_read)
{
	int	nbr;
	int	flag;

	nbr = 0;
	flag = -1;
	while (*line_read)
	{
		else if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (!is_whitespace(*line_read) && *line_read != '|')
			flag = 0;
		else if (*line_read == '|')
		{
			if (flag == -1 || check_syntax_around_pipe(&line_read) == -1)
			{
				ft_dprintf(STDERR_FILENO,
					"syntax error near unexpected token `%c'", '|');
				return (-2);
			}
			nbr++;
			flag = -1;
			continue ;
		}
		line_read++;
	}
	if (flag == 1 || flag == 2)
		ft_dprintf(STDERR_FILENO, "syntax error near quotes\n");
	return (nbr);
}

int	check_empty_line(char *line_read)
{
	while (*line_read && is_whitespace(*line_read))
		line_read++;
	if (!*line_read)
		return (-1);
	return (0);
}

void	init_struct_array(t_command **cmd_arr, int cmd_nbr)
{
	int	i;

	i = -1;
	while (++i < cmd_nbr)
	{
		cmd_arr[i]->here_doc = NULL;
		cmd_arr[i]->cmd_args = NULL;
		cmd_arr[i]->stdin_fds = -2;
		cmd_arr[i]->stdout_fds = -2;
	}
}

int	check_quotes_in_heredoc_delim(char *line_read)
{
	while (*line_read)
	{
		if (*line_read == 39 || *line_read == '"')
			return (1);
		line_read++;
	}
	return (0);
}

int	count_delim_len(char *line_read)
{
	int	delim_len;
	int	flag;

	delim_len = 0;
	flag = 0;
	while (*line_read)
	{
		if ((flag == 1 && *line_read == 39)
				|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			delim_len++;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (!is_whitespace(*line_read))
			delim_len++;
		else if (is_whitespace(*line_read))
			break ;
		line_read++;
	}
	return (delim_len);
}

char	*alloc_delim_str(char *line_read, int delim_len)
{
	int		i;
	char	*delim;

	i = -1;
	delim = malloc(sizeof(char) * (delim_len + 1));
	if (!delim)
		return (NULL);
	delim[delim_len] = '\0';
	while (*line_read)
	{
		else if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			delim[++i] = *line_read;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (!is_whitespace(*line_read))
			delim[++i] = *line_read;
		else if (is_whitespace(*line_read))
			break ;
		line_read++;
	}
}

int	ft_heredoc(t_params *params, char *delim, int flag)
{
	static int	nbr = 0;
	int		fd1;
	int		fd2;
	char	*here_doc;
	char	*str;

	here_doc = strjoin_and_free_str(".tmpheredoc", ft_itoa(nbr++), 2);
	fd1 = open(here_doc, O_WRONLY | O_TRUNC | O_CREAT, 0600);
	while (1)
	{
		str = get_next_line(STDIN_FILENO);
		if (!str)
			break ;
		if (my_strcmp(delim, str))
		{
			fd2 = open(heredoc, O_RDONLY);
			close(fd1);
			free(delim);
			free(str);
		}
		if (flag)
			str = alloc_new_heredoc(str, params);
		write(fd1, str, my_strlen(str));
		free(str);
	}
}

int	is_heredoc_rightmost(char *line_read, int delim_len)
{
	int	flag;

	flag = 0;
	if (*line_read == 39 || *line_read == '"')
	{
		line_read++;
		flag = 1;
	}
	line_read += delim_len;
	if (flag)
		line_read++;
	while (*line_read)
	{
		if ((flag == 2 && *line_read == 39)
			|| (flag == 3 && *line_read == '"'))
			flag = 0;
		else if (flag == 2 || flag == 3)
			;
		else if (*line_read == 39)
			flag = 2;
		else if (*line_read == '"')
			flag = 3;
		else if (*line_read == '<')
			return (0);
		else if (*line_read == '|')
			return (1);
		line_read++;
	}
	return (1);
}

void	check_for_heredoc(t_params *params, int i, char **line_read)
{
	int		delim_len;
	int		flag;
	char	*delim;

	delim = NULL;
	if (*(*line_read + 1) == '<')
	{
		while (**line_read && is_whitespace(**line_read))
			(*line_read)++;
		flag = check_quotes_in_heredoc_delim(*line_read);
		delim_len = count_delim_len(*line_read);
		delim = alloc_delim_str(*line_read, delim_len);
		params->cmd_arr[i].heredoc_rightmost = is_heredoc_rightmost(*line_read, delim_len);
		if (delim)
			params->cmd_arr[i].stdin_fds = ft_heredoc(params, delim, flag);
		flag = 0;
		if (**line_read == 39 || **line_read == '"')
		{
			flag = 1;
			**line_read = ' ';
			(*line_read)++;
		}
		while (++j < delim_len)
		{
			**line_read = ' ';
			(*line_read)++;
		}
		if (flag)
			**line_read = ' ';
	}
}

void	open_all_heredocs(t_params *params, char *line_read)
{
	int	i;

	i = 0;
	while (*line_read)
	{
		if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (*line_read == '<')
		{
			if (*(line_read + 1))
			{
				check_for_heredoc(params, i, &line_read)
				continue ;
			}
		}
		else if (*line_read == '|')
			i++;
		line_read++;
	}
}

void	count_meta_char_len(char **line_read, int *new_len)
{
	if (**line_read == '|')
	{
		(*line_read)++;
		(*new_len)++;
	}
	else if ((**line_read == '<' && *(*line_read + 1) == '<')
		|| (**line_read == '>' && *(*line_read + 1) == '>'))
	{
		*line_read += 2;
		*new_len += 2;
	}
	else if (**line_read == '<' || **line_read == '>')
	{
		(*line_read)++;
		*new_len += 1;
	}
}

void	count_var_len2(char **temp, int *token_len, t_env *head_env, int j)
{
	if ((*temp)[0] && j != 0)
	{
		if (!head_env || (!(((*temp)[0] >= 'a' && (*temp)[0] <= 'z')
			|| ((*temp)[0] >= 'A' && (*temp)[0] <= 'Z'))
			&& ((*temp)[0] != '_') && ((*temp)[0] != '?')))
		{
			(*temp) += j;
			return ;
		}
		(*token_len) += my_strlen(head_env->value);
	}
	else
		(*token_len)++;
	(*temp) += j;
}

void	count_var_len(char **temp, int *token_len, t_env *head_env)
{
	int	j;

	j = 0;
	(*temp)++;
	while (!is_whitespace((*temp)[j]) && (*temp)[j] != '|'
		&& (*temp)[j] != '<' && (*temp)[j] != '>' && (*temp)[j]
		&& (*temp)[j] != 39 && (*temp)[j] != '"' && (*temp)[j] != '$')
		if ((*temp)[j++] == '?')
			break ;
	while (head_env)
	{
		if (!my_strncmp(*temp, head_env->key, j))
			break ;
		head_env = head_env->next;
	}
	count_var_len2(temp, token_len, head_env, j);
}

int	count_new_line_len(t_params *params, char *line_read)
{
	int		new_len;
	int		flag;

	new_len = 0;
	flag = -1;
	while (*line_read)
	{
		if (!is_whitespace(*line_read) && flag == -2)
		{
			new_len++;
			flag = 0;
		}
		if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (*line_read == '$' && flag != 1)
			count_var_len(&line_read, &new_len, *(params->head_env));
		else if (flag == 1 || flag == 2)
			new_len++;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (*line_read == '|' || *line_read == '<' || *line_read == '>')
			count_meta_char_len(&line_read, &new_len);
		else if (!is_whitespace(*line_read))
		{
			flag = 0;
			new_len++;
		}
		else if (is_whitespace(*line_read))
			if (flag != -1)
				flag = -2;
		}
		line_read++;
	}
	return (new_len);
}

int	move_ptr_past_var(char **temp, t_env **head_env)
{
	int	j;

	j = 0;
	while (!is_whitespace((*temp)[j]) && (*temp)[j] != '|' && (*temp)[j]
		&& (*temp)[j] != '<' && (*temp)[j] != '>' && (*temp)[j] != 39
		&& (*temp)[j] != '"' && (*temp)[j] != '$')
		j++;
	while (*head_env)
	{
		if (!my_strncmp(*temp, (*head_env)->key, j))
			break ;
		*head_env = (*head_env)->next;
	}
//	if (!*head_env)
//	{
//		(*temp) += j;
//		return (-1);
//	}
	return (j);
}

void	copy_var_len(char **temp, char *token_arr, int *i, t_env *head_env)
{
	int	j;
	int	k;

	k = 0;
	(*temp)++;
	j = move_ptr_past_var(temp, &head_env);
//	if (j == -1)
//		return ;
	if ((*temp)[0] && j != 0)
	{
		if (!head_env)
		{
			(*temp) += j;
			return ;
		}
		if (!(((*temp)[0] >= 'a' && (*temp)[0] <= 'z')
			|| ((*temp)[0] >= 'A' && (*temp)[0] <= 'Z'))
			&& ((*temp)[0] != '_') && ((*temp)[0] != '?'))
		{
			token_arr[++(*i)] = '$'; 
			(*temp) += j;
			return ;
		}
		while (k < my_strlen(head_env->value))
			token_arr[++(*i)] = head_env->value[k++];
	}
	else
		token_arr[++(*i)] = '$';
	(*temp) += j;
}

void	copy_lineread_to_newline(t_params *params, char *line_read, char *new_line)
{
	int	flag;
	int	j;
	int	i;

	flag = 0;
	j = -1;
	i = 0;
	while (*line_read)
	{
		if (!is_whitespace(*line_read) && flag == -2)
		{
			new_line[++j] = ' ';
			flag = 0;
		}
		if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (*line_read == '$' && flag != 1)
			copy_var_len(&line_read, new_line, &j, *(params->head_env));
		else if (flag == 1 || flag == 2)
			new_line[++j] = *line_read;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (*line_read == '|' || *line_read == '<' || *line_read == '>')
			copy_meta_char_len(&line_read, &j, new_line);
		else if (!is_whitespace(*line_read))
		{
			flag = 0;
			new_line[++j] = *line_read;
		}
		else if (is_whitespace(*line_read))
			if (flag != -1)
				flag = -2;
		}
		line_read++;
	}
	return (new_line);
}

char	*expand_vars_and_remove_quotes(t_params *params, char *line_read)
{
	int		new_len;
	char	*temp;
	char	*new_line;

	temp = line_read;
	new_len = count_new_line_len(params, temp);
	new_line = malloc(sizeof(char) * (new_len + 1));
	if (!new_line)
		return (NULL);
	new_line[new_len] = '\0';
	temp = line_read;
	copy_lineread_to_newline(params, temp, new_line);
	free(line_read);
	return (new_line);
}

/*
void	store_files_for_simple_cmd(t_params *params, int i, char **line_read)
{
	if (*line_read == '>' && *(*line_read + 1) == '>')
		a
}

void	store_files_for_child_processes(t_params *params, char *line_read)
{
	int	flag;
	int	i;

	flag = 0;
	i = 0;
	while (*line_read)
	{
		if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (*line_read == '<' || *line_read == '>')
			store_files_for_simple_cmd(params, i, &line_read);
		else if (*line_read == '|')
			i++;
	}
}
*/

int	generate_struct_array(t_params *params, char *line_read)
{
	char	*temp;
	int		cmd_nbr;
	t_command	*cmd_arr;
	char	***execve_args;

	temp = line_read;
	cmd_nbr = nbr_of_pipes(temp) + 1;
	if (cmd_nbr == -1 || check_redir_syntax(temp) == -1
			|| check_empty_line(temp) == -1)
		return (-1);
	cmd_arr = malloc(sizeof(t_command) * cmd_nbr);
	execve_args = malloc(sizeof(char **) * (cmd_nbr + 1));
	if (!cmd_arr || !execve_args)
		return (-1);
	execve_args[cmd_nbr] = NULL;
	init_struct_array(cmd_arr, cmd_nbr);
	params->cmd_arr = cmd_arr;
	params->cmd_nbr = cmd_nbr;
	open_all_heredocs(params, temp);
	line_read = expand_vars_and_remove_quotes(params, line_read);
	//temp = line_read;
	//store_files_for_child_process(params, temp);
}
