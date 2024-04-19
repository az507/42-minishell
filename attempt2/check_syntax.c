/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 16:04:50 by achak             #+#    #+#             */
/*   Updated: 2024/04/18 17:41:02 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_after_meta_char2(char **line_read)
{
	if (*(*line_read + 1) == '<' || *(*line_read + 1) == '>')
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
	return (0);
}

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
		else
			return (check_after_meta_char2(line_read));
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
		if (*line_read == 39 || *line_read == '"')
		{
			skip_til_end_of_quotes(&line_read);
			continue ;
		}
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
