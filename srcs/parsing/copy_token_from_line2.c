/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_token_from_line2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 19:48:11 by achak             #+#    #+#             */
/*   Updated: 2024/04/17 18:57:27 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	copy_meta_char(char **temp, char *token_arr, int *i)
{
	token_arr[++(*i)] = 39;
	if (**temp == '|')
		token_arr[++(*i)] = **temp;
	// old version: else if (*temp + 1)
	else if (*(temp + 1))
	{
		if (*(*temp + 1) != '<' && *(*temp + 1) != '>')
			token_arr[++(*i)] = **temp;
		else if ((**temp == '>' && *(*temp + 1) == '>')
			|| (**temp == '<' && *(*temp + 1) == '<'))
		{
			token_arr[++(*i)] = **temp;
			(*temp)++;
			token_arr[++(*i)] = **temp;
		}
		else
			token_arr[++(*i)] = **temp;
	}
	else
		token_arr[++(*i)] = **temp;
	(*temp)++;
}

int	copy_quote_len(char **temp, int *i, char *token_arr, t_env *head_env)
{
	char	quote;

	quote = **temp;
	(*temp)++;
	while (**temp != quote && **temp)
	{
		if (**temp == '$' && quote == '"')
		{
			copy_var_len(temp, token_arr, i, head_env);
			continue ;
		}
		token_arr[++(*i)] = **temp;
		(*temp)++;
	}
	if (**temp == quote)
		(*temp)++;
	return (1);
}

/*
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
*/
