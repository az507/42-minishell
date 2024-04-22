/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 16:10:12 by achak             #+#    #+#             */
/*   Updated: 2024/04/21 18:14:16 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	save_heredoc_to_struct(t_command *cmd_arr, int fd1, int fd2,
		char *heredoc)
{
	if (cmd_arr->stdin_fd != -2)
	{
		wrapper(close(cmd_arr->stdin_fd), "close");
		cmd_arr->stdin_fd = -2;
		wrapper(unlink(cmd_arr->heredoc), "unlink");
		free(cmd_arr->heredoc);
		cmd_arr->heredoc = NULL;
	}
	fd2 = open(heredoc, O_RDONLY);
	cmd_arr->stdin_fd = fd2;
	cmd_arr->heredoc = heredoc;
	close(fd1);
	return (0);
}

int	check_heredoc_eof(char *str, char *delim)
{
	if (!str)
	{
		ft_dprintf(STDERR_FILENO,
			"warning: here_document delimited by end-of-file (wanted `%s')\n",
			delim);
		free(delim);
		return (1);
	}
	if (my_strcmp(delim, str))
	{
		free(str);
		free(delim);
		return (1);
	}
	return (0);
}

int	ft_heredoc(t_params *params, int i, char *delim, int flag)
{
	static int	nbr = 0;
	int			fd1;
	int			fd2;
	char		*heredoc;
	char		*str;

	heredoc = strjoin_and_free_str(".tmpheredoc", ft_itoa(nbr++), 2);
	fd1 = open(heredoc, O_WRONLY | O_TRUNC | O_CREAT, 0600);
	fd2 = -2;
	heredoc_signal_handler();
	while (1)
	{
		str = get_next_line(STDIN_FILENO);
		free_heredoc_stuff(str, heredoc, delim, fd1);
		if (g_async_flag == 1)
			return (cleanup_before_exiting_heredoc(params, i));
		if (check_heredoc_eof(str, delim) == 1)
			return (save_heredoc_to_struct
				(&params->cmd_arr[i], fd1, fd2, heredoc));
		if (!flag)
			str = alloc_new_heredoc(str, params);
		write(fd1, str, my_strlen(str));
		free(str);
	}
	return (0);
}

void	ignore_quotes_finding_other_heredocs(char **line_read, int *flag)
{
	while (**line_read)
	{
		if ((*flag == 2 && **line_read == 39)
			|| (*flag == 3 && **line_read == '"'))
		{
			(*line_read)++;
			return ;
		}
		else if (*flag == 2 || *flag == 3)
			;
		else if (**line_read == 39)
			*flag = 2;
		else if (**line_read == '"')
			*flag = 3;
		(*line_read)++;
	}
}

int	is_heredoc_rightmost(char *line_read, int delim_len)
{
	int	flag;

	flag = 0;
	if (*line_read == 39 || *line_read == '"')
	{
		line_read++;
		flag = 1;
	}
	line_read += delim_len;
	if (flag)
		line_read++;
	while (*line_read)
	{
		if (*line_read == 39 || *line_read == '"')
		{
			ignore_quotes_finding_other_heredocs(&line_read, &flag);
			continue ;
		}
		else if (*line_read == '<')
			return (0);
		else if (*line_read == '|')
			return (1);
		line_read++;
	}
	return (1);
}
