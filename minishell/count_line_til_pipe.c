/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_line_til_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:31:32 by achak             #+#    #+#             */
/*   Updated: 2024/04/18 18:33:40 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	count_chars_in_quoted_str(char **temp, int *count)
{
	int	flag;

	flag = 0;
	while (**temp)
	{
		(*count)++;
		if ((flag == 1 && **temp == 39)
			|| (flag == 2 && **temp == '"'))
		{
			(*temp)++;
			return ;
		}
		else if (flag)
			;
		else if (**temp == 39)
			flag = 1;
		else if (**temp == '"')
			flag = 2;
		(*temp)++;
	}
}

int	count_len_til_pipe2(char **temp, int *flag, int *count)
{
	if (**temp == '|')
		return (1);
	else if (!is_whitespace(**temp))
	{
		*flag = 0;
		(*count)++;
	}
	else if (is_whitespace(**temp))
		if (*flag != -1)
			*flag = -2;
	return (0);
}

int	count_len_til_pipe(char *temp)
{
	int	count;
	int	flag;

	count = 0;
	flag = -1;
	while (*temp)
	{
		if (flag == -2 && !is_whitespace(*temp))
		{
			count++;
			flag = 0;
		}
		if (*temp == 39 || *temp == '"')
		{
			count_chars_in_quoted_str(&temp, &count);
			continue ;
		}
		else if (count_len_til_pipe2(&temp, &flag, &count) == 1)
			break ;
		temp++;
	}
	return (count);
}
