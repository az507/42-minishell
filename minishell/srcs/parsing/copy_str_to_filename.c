/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_str_to_filename.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 13:29:59 by achak             #+#    #+#             */
/*   Updated: 2024/04/19 15:46:54 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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
			if (j == 0 && is_numeric((*temp)[j]))
				break ;
			j++;
		}
		copy_valid_var
			(is_valid_var(*(params->head_env), *temp, j), filename, k);
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
	else if (is_whitespace(*temp) || *temp == '<' || *temp == '>'
		|| *temp == '|')
		return (0);
	else if (!is_whitespace(*temp))
		filename[(*k)++] = *temp;
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
