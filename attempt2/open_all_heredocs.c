/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_all_heredocs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 16:46:40 by achak             #+#    #+#             */
/*   Updated: 2024/04/21 14:57:28 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	erase_quoted_portion_in_heredoc(char **line_read, int *flag)
{
	while (**line_read)
	{
		if ((*flag == 1 && **line_read == 39)
			|| (*flag == 2 && **line_read == '"'))
		{
			*flag = 0;
			**line_read = ' ';
			(*line_read)++;
			return ;
		}
		else if (*flag)
			;
		else if (**line_read == 39)
			*flag = 1;
		else if (**line_read == '"')
			*flag = 2;
		**line_read = ' ';
		(*line_read)++;
	}
}

void	erase_heredoc_and_delim(char **line_read, int *flag)
{
	while (**line_read && **line_read == '<')
	{
		**line_read = ' ';
		(*line_read)++;
	}
	while (**line_read && is_whitespace(**line_read))
	{
		**line_read = ' ';
		(*line_read)++;
	}
	while (**line_read)
	{
		if (**line_read == 39 || **line_read == '"')
		{
			erase_quoted_portion_in_heredoc(line_read, flag);
			continue ;
		}
		else if (is_whitespace(**line_read) || **line_read == '<'
			|| **line_read == '>' || **line_read == '|')
			break ;
		else if (!is_whitespace(**line_read))
			**line_read = ' ';
		(*line_read)++;
	}
}

int	check_for_heredoc(t_params *params, int i, char **line_read)
{
	int		delim_len;
	int		flag;
	int		j;
	char	*delim;

	j = 0;
	delim = NULL;
	if (*(*line_read + 1) == '<')
	{
		while (**line_read && is_whitespace(**line_read))
			(*line_read)++;
		flag = check_quotes_in_heredoc_delim(*line_read);
		delim_len = count_delim_len(*line_read);
		delim = alloc_delim_str(*line_read, delim_len);
		params->cmd_arr[i].heredoc_rightmost
			= is_heredoc_rightmost(*line_read, delim_len);
		if (delim)
			if (ft_heredoc(params, i, delim, flag) == -1)
				return (-1);
		flag = 0;
		erase_heredoc_and_delim(line_read, &flag);
	}
	else
		(*line_read)++;
	return (0);
}

void	ignore_quotes_looking_for_heredoc(char **line_read, int *flag)
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
			;
		else if (**line_read == 39)
			*flag = 1;
		else if (**line_read == '"')
			*flag = 2;
		(*line_read)++;
	}
}

int	open_all_heredocs(t_params *params, char *line_read)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (*line_read)
	{
		if (*line_read == 39 || *line_read == '"')
		{
			ignore_quotes_looking_for_heredoc(&line_read, &flag);
			continue ;
		}
		else if (*line_read == '<' && *(line_read + 1))
		{
			if (check_for_heredoc(params, i, &line_read) == -1)
				return (-1);
			continue ;
		}
		else if (*line_read == '|')
			i++;
		line_read++;
	}
	return (0);
}

/*
			if (*(line_read + 1))
			{
				if (check_for_heredoc(params, i, &line_read) == -1)
					return (-1);
				continue ;
			}
		// if global var async == 1
		// close all open fds, prepare to cleanup
		// to move to next iteration of readline while loop
//		if (async == 1)
//		{
//			i = -1;
//			while (++i < params->cmd_nbr)
//			{
//				if (params->cmd_arr[i].stdin_fd != -2)
//					close(params->cmd_arr[i].stdin_fd);
//			}
//			cleanup_params(params);
//			async = 0;
//			return (-1);
//		}
		// ASYNC SIGINT
*/
