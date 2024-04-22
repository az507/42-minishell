/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_token_len.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 09:40:12 by achak             #+#    #+#             */
/*   Updated: 2024/04/22 15:32:24 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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

void	dont_count_if_invalid_var(char **temp, int j, int *token_len)
{
	if (!is_alphabet(**temp) && **temp != '_' && **temp != '?' && j == 0)
	{
		(*token_len)++;
		return ;
	}
	*temp += j;
	if (**temp && is_whitespace(**temp) && j != 0)
	{
		while (**temp && is_whitespace(**temp))
			(*temp)++;
	}
}

void	count_var_len(char **temp, int *token_len, t_env *head_env, int flag)
{
	int	j;

	j = 0;
	(*temp)++;
	while ((*temp)[j] && (is_alphabet((*temp)[j]) || is_numeric((*temp)[j])
		|| (*temp)[j] == '_' || (*temp)[j] == '?'))
	{
		if ((is_numeric((*temp)[j]) && j == 0)
			|| ((*temp)[j] == '?'))
		{
			if ((*temp)[j] == '?' && j == 0)
				j = 1;
			break ;
		}
		j++;
	}
	while (head_env)
	{
		if (!my_strncmp(*temp, head_env->key, j))
			break ;
		head_env = head_env->next;
	}
	if ((!head_env || j == 0) && flag == 0 && *token_len == 0)
		return (dont_count_if_invalid_var(temp, j, token_len));
	count_var_len2(temp, token_len, head_env, j);
}

void	count_regular_len(char **temp, int *token_len)
{
	while (!is_whitespace(**temp) && **temp != '|' && **temp != '<'
		&& **temp != '>' && **temp != 39 && **temp != '"' && **temp
		&& **temp != '$')
	{
		(*temp)++;
		(*token_len)++;
	}
}
