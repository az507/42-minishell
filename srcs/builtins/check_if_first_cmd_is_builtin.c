/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_first_cmd_is_builtin.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:34:54 by achak             #+#    #+#             */
/*   Updated: 2024/04/18 16:39:22 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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

int	check_if_fork_needed(t_params *params, char *line_read)
{
	if (params->cmd_nbr > 1)
		return (1);
	if (check_if_first_cmd_is_builtin(line_read, *(params->head_env)) == 0)
		return (1);
	else
		return (0);
}

void	skip_redir_and_filename(char **line_read)
{
	int	flag;

	flag = 0;
	while (**line_read == '<' || **line_read == '>'
		|| is_whitespace(**line_read))
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

int	determine_whats_first_word(char *line_read, t_env *head_env)
{
	int		len;
	int		rv;
	char	*word;

	len = count_token_len(line_read, head_env);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (-1);
	word[len] = '\0';
	copy_token_from_line(&line_read, word, head_env);
	rv = check_if_cmd_is_builtin(word);
	free(word);
	return (rv);
}

int	check_if_first_cmd_is_builtin(char *line_read, t_env *head_env)
{
	int		flag;
	char	*temp;

	flag = 0;
	temp = line_read;
	while (*temp)
	{
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
	return (0);
}
