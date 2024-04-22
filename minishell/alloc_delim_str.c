/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_delim_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:11:23 by achak             #+#    #+#             */
/*   Updated: 2024/04/19 16:29:10 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	copy_quoted_parts_in_delim(char **line_read, char *delim, int *flag,
		int *i)
{
	while (**line_read)
	{
		if ((*flag == 1 && **line_read == 39)
			|| (*flag == 2 && **line_read == '"'))
		{
			*flag = 0;
			(*line_read)++;
			return ;
		}
		else if (*flag)
			delim[++(*i)] = **line_read;
		else if (**line_read == 39)
			*flag = 1;
		else if (**line_read == '"')
			*flag = 2;
		(*line_read)++;
	}
}

int	copy_delim_str_redir(char **line_read, char *delim, int *flag, int *i)
{
	if (*(*line_read + 1) == '<')
	{
		*line_read += 2;
		while (**line_read && is_whitespace(**line_read))
			(*line_read)++;
		while (**line_read)
		{
			if (**line_read == 39 || **line_read == '"')
			{
				copy_quoted_parts_in_delim(line_read, delim, flag, i);
				continue ;
			}
			else if (is_whitespace(**line_read) || **line_read == '<'
				|| **line_read == '>' || **line_read == '|')
				break ;
			else if (!is_whitespace(**line_read))
				delim[++(*i)] = **line_read;
			(*line_read)++;
		}
		return (1);
	}
	return (0);
}
/*
//			if (*(line_read + 1) == '<')
//			{
//				line_read += 2;
//				while (*line_read && is_whitespace(*line_read))
//					line_read++;
//				while (*line_read)
//				{
//					if ((flag == 1 && *line_read == 39)
//						|| (flag == 2 && *line_read == '"'))
//						flag = 0;
//					else if (flag == 1 || flag == 2)
//						delim[++i] = *line_read;
//					else if (*line_read == 39)
//						flag = 1;
//					else if (*line_read == '"')
//						flag = 2;
//					else if (!is_whitespace(*line_read))
//						delim[++i] = *line_read;
//					else if (is_whitespace(*line_read))
//						break ;
//					line_read++;
//				}
//				return (delim);
//			}
*/

void	copy_delim_str(char *line_read, char *delim, int flag, int i)
{
	while (*line_read)
	{
		if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			delim[++i] = *line_read;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (*line_read == '<')
		{
			if (copy_delim_str_redir(&line_read, delim, &flag, &i) == 1)
				return ;
		}
		else if (is_whitespace(*line_read))
			break ;
		line_read++;
	}
}
/*
	//			return (delim);
//	while (*line_read)
//	{
//		// if (*line_read == 39 || *line_read == '"')
//		// {
//		// 	copy_delim_in_quotes(&line_read, delim, &flag, &i);
//		// 	continue ;
//		// }
//		if ((flag == 1 && *line_read == 39)
//			|| (flag == 2 && *line_read == '"'))
//			flag = 0;
//		else if (flag == 1 || flag == 2)
//			delim[++i] = *line_read;
//		else if (*line_read == 39)
//			flag = 1;
//		else if (*line_read == '"')
//			flag = 2;
//		else if (*line_read == '<')
//			if (copy_delim_str_redir(&line_read, delim, &flag, &i) == 1)
//				return (delim);
//		else if (is_whitespace(*line_read))
//			break ;
//		line_read++;
//	}
//	return (delim);
*/

char	*alloc_delim_str(char *line_read, int delim_len)
{
	int		i;
	int		flag;
	char	*delim;

	i = -1;
	flag = 0;
	delim = malloc(sizeof(char) * (delim_len + 1));
	if (!delim)
		return (NULL);
	delim[delim_len] = '\0';
	copy_delim_str(line_read, delim, flag, i);
	return (delim);
}
