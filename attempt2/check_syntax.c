/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 16:04:50 by achak             #+#    #+#             */
/*   Updated: 2024/04/10 13:27:23 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_after_meta_char(char **line_read)
{
	if (*(*line_read + 1))
	{
		if (**line_read == '<' && *(*line_read + 1) == '<')
		{
			(*line_read) += 2;
			while (**line_read && is_whitespace(**line_read))
				(*line_read)++;
			if (!**line_read)
				return (-1);
			else if (**line_read == '<' || **line_read == '>'
				|| **line_read == '|')
				return (-1);
			return (0);
		}
		else if (*(*line_read + 1) == '<' || *(*line_read + 1) == '>')
		{
			if (**line_read != *(*line_read + 1))
				return (-1);
			(*line_read) += 2;
			return (0);
		}
		else
		{
			(*line_read)++;
			while (**line_read && is_whitespace(**line_read))
				(*line_read)++;
			if (**line_read == '<' || **line_read == '>'
				|| **line_read == '|')
				return (-1);
			return (0);
		}
	}
	else
		return (-1);
	return (0);
}

int	check_redir_syntax(char *line_read)
{
	int	flag;

	flag = 0;
	while (*line_read)
	{
		if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (*line_read == '<' || *line_read == '>')
		{
			if (check_after_meta_char(&line_read) == -1)
			{
				ft_dprintf(STDERR_FILENO,
					"syntax error near unexpected token `%c'\n",
						*line_read);
				return (-1);
			}
			continue ;
		}
		line_read++;
	}
	return (0);
}

int	check_syntax_around_pipe(char **line_read)
{
	if (*(*line_read + 1))
	{
		(*line_read)++;
		while (**line_read && is_whitespace(**line_read))
			(*line_read)++;
		if (**line_read)
		{
			if (**line_read == '|')
				return (-1);
			return (0);
		}
		else
			return (-1);
	}
	return (-1);
}

int	nbr_of_pipes(char *line_read)
{
	int	nbr;
	int	flag;

	nbr = 0;
	flag = -1;
	while (*line_read)
	{
		if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (!is_whitespace(*line_read) && *line_read != '|')
			flag = 0;
		else if (*line_read == '|')
		{
			if (flag == -1 || check_syntax_around_pipe(&line_read) == -1)
			{
				ft_dprintf(STDERR_FILENO,
					"syntax error near unexpected token `%c'\n", '|');
				return (-2);
			}
			nbr++;
			flag = -1;
			continue ;
		}
		line_read++;
	}
//	if (flag == 1 || flag == 2)
//		ft_dprintf(STDERR_FILENO, "syntax error near quotes\n");
	return (nbr);
}

int	check_empty_line(char *line_read)
{
	while (*line_read && is_whitespace(*line_read))
		line_read++;
	if (!*line_read)
		return (-1);
	return (0);
}
