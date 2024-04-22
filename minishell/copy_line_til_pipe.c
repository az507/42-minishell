/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_line_til_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:32:19 by achak             #+#    #+#             */
/*   Updated: 2024/04/18 18:33:17 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
/*
//		if ((flag == 1 && *temp == 39)
//			|| (flag == 2 && *temp == '"'))
//		{
//			flag = 0;
//			count++;
//		}
//		else if (flag == 1 || flag == 2)
//			count++;
//		else if (*temp == 39)
//		{
//			flag = 1;
//			count++;
//		}
//		else if (*temp == '"')
//		{
//			flag = 2;
//			count++;
//		}
///////////////////////////////////////////////////////////////////////
//		if ((flag == 1 && **temp == 39)
//			|| (flag == 2 && **temp == '"'))
//		{
//			flag = 0;
//			params->cmd_arr[i].raw_str[++j] = **temp;
//		}
//		else if (flag == 1 || flag == 2)
//			params->cmd_arr[i].raw_str[++j] = **temp;
//		else if (**temp == 39)
//		{
//			flag = 1;
//			params->cmd_arr[i].raw_str[++j] = **temp;
//		}
//		else if (**temp == '"')
//		{
//			flag = 2;
//			params->cmd_arr[i].raw_str[++j] = **temp;
//		}
//		/ \\\\
//		else if (**temp == '|')
//			break ;
//		else if (!is_whitespace(**temp))
//		{
//			flag = 0;
//			params->cmd_arr[i].raw_str[++j] = **temp;
//		}
//		else if (is_whitespace(**temp))
//			if (flag != -1)
//				flag = -2;
*/

void	copy_chars_in_quoted_str(t_params *params, int i, char **temp, int *j)
{
	int	flag;

	flag = 0;
	while (**temp)
	{
		params->cmd_arr[i].raw_str[++(*j)] = **temp;
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

int	copy_line_til_pipe2(char *raw_str, int *j, char **temp, int *flag)
{
	if (**temp == '|')
		return (1);
	else if (!is_whitespace(**temp))
	{
		*flag = 0;
		raw_str[++(*j)] = **temp;
	}
	else if (is_whitespace(**temp))
		if (*flag != -1)
			*flag = -2;
	return (0);
}

void	copy_line_til_pipe(t_params *params, int i, char **temp)
{
	int	j;
	int	flag;

	j = -1;
	flag = -1;
	while (**temp)
	{
		if (flag == -2 && !is_whitespace(**temp))
		{
			params->cmd_arr[i].raw_str[++j] = ' ';
			flag = 0;
		}
		if (**temp == 39 || **temp == '"')
		{
			copy_chars_in_quoted_str(params, i, temp, &j);
			continue ;
		}
		else if (copy_line_til_pipe2(params->cmd_arr[i].raw_str,
				&j, temp, &flag) == 1)
			break ;
		(*temp)++;
	}
}
