/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_delim_len.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:12:17 by achak             #+#    #+#             */
/*   Updated: 2024/04/19 16:12:59 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_quotes_in_heredoc_delim(char *line_read)
{
	while (*line_read && (is_whitespace(*line_read) || *line_read == '<'))
		line_read++;
	while (*line_read)
	{
		if (*line_read == 39 || *line_read == '"')
			return (1);
		if (is_whitespace(*line_read))
			break ;
		line_read++;
	}
	return (0);
}

void	go_thru_quotes_for_delim(char **line_read, int *delim_len, int *flag)
{
	while (**line_read)
	{
		if ((*flag == 1 && **line_read == 39)
			|| (*flag == 2 && **line_read == '"'))
		{
			(*line_read)++;
			return ;
		}
		else if (*flag)
			(*delim_len)++;
		else if (**line_read == 39)
			*flag = 1;
		else if (**line_read == '"')
			*flag = 2;
		(*line_read)++;
	}
}

int	count_delim_len_redir(char **line_read, int *delim_len, int *flag)
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
				go_thru_quotes_for_delim(line_read, delim_len, flag);
				continue ;
			}
			else if (is_whitespace(**line_read) || **line_read == '<'
				|| **line_read == '>' || **line_read == '|')
				break ;
			else if (!is_whitespace(**line_read))
				(*delim_len)++;
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
//						delim_len++;
//					else if (*line_read == 39)
//						flag = 1;
//					else if (*line_read == '"')
//						flag = 2;
//					else if (!is_whitespace(*line_read))
//						delim_len++;
//					else if (is_whitespace(*line_read))
//						break ;
//					line_read++;
//				}
//				return (delim_len);
//			}
*/

int	count_delim_len(char *line_read)
{
	int	delim_len;
	int	flag;

	delim_len = 0;
	flag = 0;
	while (*line_read)
	{
		if ((flag == 1 && *line_read == 39) || (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			delim_len++;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (*line_read == '<')
		{
			if (count_delim_len_redir(&line_read, &delim_len, &flag) == 1)
				return (delim_len);
		}
		else if (is_whitespace(*line_read))
			break ;
		line_read++;
	}
	return (delim_len);
}
