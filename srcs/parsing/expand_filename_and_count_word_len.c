/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_filename_and_count_word_len.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 13:26:49 by achak             #+#    #+#             */
/*   Updated: 2024/04/19 16:30:31 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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
			if ((j != 0 && (*temp)[j] == '?')
				|| (j == 0 && is_numeric((*temp)[j])))
				break ;
			j++;
		}
		count_valid_var
			(is_valid_var(*(params->head_env), *temp, j), word_len);
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
	else if (is_whitespace(*temp) || *temp == '<' || *temp == '>'
		|| *temp == '|')
		return (0);
	else if (!is_whitespace(*temp))
		(*word_len)++;
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
