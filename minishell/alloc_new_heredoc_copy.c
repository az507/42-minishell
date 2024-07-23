/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_new_heredoc_copy.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:06:20 by achak             #+#    #+#             */
/*   Updated: 2024/04/18 16:06:40 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	look_for_env_var(char *str)
{
	int	flag;

	flag = 0;
	while (*str)
	{
		if (flag && (is_alphabet(*str) || *str == '_'
				|| *str == '?'))
			return (1);
		else
			flag = 0;
		if (*str == '$')
			flag = 1;
		str++;
	}
	return (0);
}

void	copy_heredoc_for_var(char **str, char *here_doc, int *i, t_env *temp)
{
	int	j;
	int	k;

	j = 1;
	k = 0;
	if ((*str)[j])
	{
		while ((*str)[j] && (is_alphabet((*str)[j])
			|| is_numeric((*str)[j]) || (*str)[j] == '_'))
			j++;
	}
	while (temp)
	{
		if (!my_strncmp(*str, temp->key, j))
			break ;
		temp = temp->next;
	}
	if (temp)
		if (temp->value)
			while (temp->value[k])
				here_doc[++(*i)] = temp->value[k++];
	*str += j - 1;
}
/*
//				j = 1;
//				if (str[j])
//				{
//					while (str[j] && (is_alphabet(str[j])
//						|| is_numeric(str[j]) || str[j] == '_'))
//						j++;
//				}
//				temp = *(params->head_env);
//				while (temp)
//				{
//					if (!my_strncmp(str, temp->key, j))
//						break ;
//					temp = temp->next;
//				}
//				if (temp)
//				{
//					if (temp->value)
//					{
//						k = 0;
//						while (temp->value[k])
//							here_doc[++i] = temp->value[k++];
//					}
//				}
//				str += j - 1;
*/

void	copy_new_heredoc(char *str, char *here_doc, t_params *params)
{
	int	i;

	i = -1;
	while (*str)
	{
		if (*str == '$' && *(str + 1))
		{
			str++;
			if (is_alphabet(*str) || *str == '_' || *str == '?')
				copy_heredoc_for_var(&str, here_doc, &i, *(params->head_env));
			else
			{
				here_doc[++i] = '$';
				here_doc[++i] = *str;
			}
		}
		else
			here_doc[++i] = *str;
		str++;
	}
}
