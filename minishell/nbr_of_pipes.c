/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nbr_of_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 17:16:53 by achak             #+#    #+#             */
/*   Updated: 2024/04/22 09:33:45 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	skip_til_end_of_quotes(char **line_read)
{
	int	flag;

	flag = 0;
	while (**line_read)
	{
		if ((flag == 1 && **line_read == 39)
			|| (flag == 2 && **line_read == '"'))
		{
			(*line_read)++;
			return ;
		}
		else if (flag)
			;
		else if (**line_read == 39)
			flag = 1;
		else if (**line_read == '"')
			flag = 2;
		(*line_read)++;
	}
}

int	count_if_valid_pipe(char **line_read, t_env **head_env,
		int *flag, int *nbr)
{
	if (*flag == -1 || check_syntax_around_pipe(line_read) == -1)
	{
		ft_dprintf(STDERR_FILENO,
			"syntax error near unexpected token `%c'\n", '|');
		update_syntax_error_exit_code(head_env);
		return (-1);
	}
	(*nbr)++;
	*flag = -1;
	return (0);
}

int	nbr_of_pipes(char *line_read, t_env **head_env)
{
	int	nbr;
	int	flag;

	nbr = 0;
	flag = -1;
	while (*line_read)
	{
		if (*line_read == 39 || *line_read == '"')
		{
			skip_til_end_of_quotes(&line_read);
			continue ;
		}
		else if (!is_whitespace(*line_read) && *line_read != '|')
			flag = 0;
		else if (*line_read == '|')
		{
			if (count_if_valid_pipe(&line_read, head_env,
					&flag, &nbr) == -1)
				return (-2);
			continue ;
		}
		line_read++;
	}
	return (nbr);
}
/*	^^^^^^^^
//	if (flag == 1 || flag == 2)
//		ft_dprintf(STDERR_FILENO, "syntax error near quotes\n");
*/

int	check_empty_line(char *line_read)
{
	while (*line_read && is_whitespace(*line_read))
		line_read++;
	if (!*line_read)
		return (-1);
	return (0);
}
