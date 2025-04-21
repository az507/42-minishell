/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_token_from_line.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:04:37 by achak             #+#    #+#             */
/*   Updated: 2024/04/22 15:34:14 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	move_ptr_past_var(char **temp, t_env **head_env)
{
	int	j;

	j = 0;
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
	while (*head_env)
	{
		if (!my_strncmp(*temp, (*head_env)->key, j))
			break ;
		*head_env = (*head_env)->next;
	}
	return (j);
}

void	dont_copy_if_invalid_var(char **temp, char *token_arr, int j, int *i)
{
	if (!is_alphabet(**temp) && **temp != '_' && **temp != '?' && j == 0)
	{
		token_arr[++(*i)] = '$';
		return ;
	}
	(*temp) += j;
	if (*i == -1 && **temp && is_whitespace(**temp) && j != 0)
	{
		while (**temp && is_whitespace(**temp))
			(*temp)++;
	}
}

// Need to account for var len for '$?' ?
void	copy_var_len(char **temp, char *token_arr, int *i, t_env *head_env)
{
	int	j;
	int	k;

	k = 0;
	(*temp)++;
	j = move_ptr_past_var(temp, &head_env);
	if (!head_env || j == 0)
		return (dont_copy_if_invalid_var(temp, token_arr, j, i));
	if ((*temp)[0] && j != 0)
	{
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

void	copy_regular_len(char **temp, char *token_arr, int *i)
{
	while (!is_whitespace(**temp) && **temp != '|' && **temp != '<'
		&& **temp != '>' && **temp != 39 && **temp != '"' && **temp
		&& **temp != '$')
	{
		token_arr[++(*i)] = **temp;
		(*temp)++;
	}
}

void	copy_token_from_line(char **temp, char *token_arr, t_env *head_env)
{
	int	i;

	i = -1;
	while (is_whitespace(**temp))
		(*temp)++;
	if (**temp == '|' || **temp == '<' || **temp == '>')
		copy_meta_char(temp, token_arr, &i);
	else
	{
		while (**temp != '|' && **temp != '<' && **temp != '>'
			&& **temp && !is_whitespace(**temp))
		{
			if (**temp == 39 || **temp == '"')
				copy_quote_len(temp, &i,
					token_arr, head_env);
			else if (**temp == '$')
				copy_var_len(temp, token_arr,
					&i, head_env);
			else if (!is_whitespace(**temp))
				copy_regular_len(temp, token_arr, &i);
		}
	}
}
