/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_all_heredocs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 16:46:40 by achak             #+#    #+#             */
/*   Updated: 2024/04/15 17:12:36 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_quotes_in_heredoc_delim(char *line_read)
{
	while (*line_read && (is_whitespace(*line_read) || *line_read == '<'))
		line_read++;
	while (*line_read)
	{
		//printf("in check quotes in heredoc delim fn: line_read = %s\n", line_read);
		if (*line_read == 39 || *line_read == '"')
			return (1);
		if (is_whitespace(*line_read))
			break ;
		line_read++;
	}
	return (0);
}

int	count_delim_len(char *line_read)
{
	int	delim_len;
	int	flag;

	delim_len = 0;
	flag = 0;
	while (*line_read)
	{
		if ((flag == 1 && *line_read == 39)
				|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			delim_len++;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (*line_read == '<')
		{
			if (*(line_read + 1) == '<')
			{
				line_read += 2;
				while (*line_read && is_whitespace(*line_read))
					line_read++;
				while (*line_read)
				{
					if ((flag == 1 && *line_read == 39)
						|| (flag == 2 && *line_read == '"'))
						flag = 0;
					else if (flag == 1 || flag == 2)
						delim_len++;
					else if (*line_read == 39)
						flag = 1;
					else if (*line_read == '"')
						flag = 2;
					else if (!is_whitespace(*line_read))
						delim_len++;
					else if (is_whitespace(*line_read))
						break ;
					line_read++;
				}
				return (delim_len);
			}
		}
//		else if (!is_whitespace(*line_read))
//			delim_len++;
		else if (is_whitespace(*line_read))
			break ;
		line_read++;
	}
	return (delim_len);
}

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
//	printf("in alloc delim str fn: *line_read = %c\n", *line_read);
//	printf("in alloc delim str fn: line_read = %s\n", line_read);
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
			if (*(line_read + 1) == '<')
			{
				line_read += 2;
				while (*line_read && is_whitespace(*line_read))
					line_read++;
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
					else if (!is_whitespace(*line_read))
						delim[++i] = *line_read;
					else if (is_whitespace(*line_read))
						break ;
					line_read++;
				}
				return (delim);
			}
		}
//		else if (!is_whitespace(*line_read))
//			delim[++i] = *line_read;
		else if (is_whitespace(*line_read))
			break ;
		line_read++;
	}
	return (delim);
}

void	save_heredoc_to_struct(t_params *params, int i, int fd2, char *heredoc)
{
	if (params->cmd_arr[i].stdin_fd != -2)
	{
		wrapper(close(params->cmd_arr[i].stdin_fd), "close");
		params->cmd_arr[i].stdin_fd = -2;
		wrapper(unlink(params->cmd_arr[i].heredoc), "unlink");
		free(params->cmd_arr[i].heredoc);
		params->cmd_arr[i].heredoc = NULL;
	}
	//printf("in save heredoc to struct fn (999)\n");
	fd2 = open(heredoc, O_RDONLY);
	params->cmd_arr[i].stdin_fd = fd2;
	//printf("fd2 = %d\n", fd2);
	params->cmd_arr[i].heredoc = heredoc;
}

void	ft_heredoc(t_params *params, int i, char *delim, int flag)
{
	static int	nbr = 0;
	int		fd1;
	int		fd2;
	char	*heredoc;
	char	*str;

	heredoc = strjoin_and_free_str(".tmpheredoc", ft_itoa(nbr++), 2);
	fd1 = open(heredoc, O_WRONLY | O_TRUNC | O_CREAT, 0600);
	fd2 = -2;
	while (1)
	{
		//printf("in heredoc loop: params->cmd_arr = %p\n", params->cmd_arr);
		str = get_next_line(STDIN_FILENO);
		//printf("delim = %s\n", delim);
		if (!str)
		{
			ft_dprintf(STDERR_FILENO, "warning: here_document delimited by end-of-file (wanted `%s')\n", delim);
//			if (params->cmd_arr[i].stdin_fd != -2)
//			{
//				wrapper(close(params->cmd_arr[i].stdin_fd), "close");
//				params->cmd_arr[i].stdin_fd = -2;
//				wrapper(unlink(params->cmd_arr[i].heredoc), "unlink");
//				free(params->cmd_arr[i].heredoc);
//				params->cmd_arr[i].heredoc = NULL;
//			}
//			fd2 = open(heredoc, O_RDONLY);
//			params->cmd_arr[i].stdin_fd = fd2;
//			params->cmd_arr[i].heredoc = heredoc;
			save_heredoc_to_struct(params, i, fd2, heredoc);
			close(fd1);
			free(delim);
			break ;
		}
		if (my_strcmp(delim, str))
		{
			//printf("encountered delim: params->cmd_arr = %p\n", params->cmd_arr);
//			if (params->cmd_arr[i].stdin_fd != -2)
//			{
//				wrapper(close(params->cmd_arr[i].stdin_fd), "close");
//				params->cmd_arr[i].stdin_fd = -2;
//				wrapper(unlink(params->cmd_arr[i].heredoc), "unlink");
//				free(params->cmd_arr[i].heredoc);
//				params->cmd_arr[i].heredoc = NULL;
//			}
//			fd2 = open(heredoc, O_RDONLY);
////			printf("fd1 in ft_heredoc loop = %d\n", fd1);
////			printf("fd2 in ft_heredoc loop = %d\n", fd2);
//			params->cmd_arr[i].stdin_fd = fd2;
	//		params->cmd_arr[i].heredoc = heredoc;
			save_heredoc_to_struct(params, i, fd2, heredoc);
			close(fd1);
			free(str);
			free(delim);
			return ;
		}
		if (!flag)
			str = alloc_new_heredoc(str, params);
		write(fd1, str, my_strlen(str));
		free(str);
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
		if ((flag == 2 && *line_read == 39)
			|| (flag == 3 && *line_read == '"'))
			flag = 0;
		else if (flag == 2 || flag == 3)
			;
		else if (*line_read == 39)
			flag = 2;
		else if (*line_read == '"')
			flag = 3;
		else if (*line_read == '<')
			return (0);
		else if (*line_read == '|')
			return (1);
		//printf("fd2 = %d\n", fd2);
		line_read++;
	}
	return (1);
}

void	erase_heredoc_and_delim(char **line_read, int *flag)
{
	//printf("in erase heredoc fn (1)\n");
	while (**line_read && (**line_read == '<' || is_whitespace(**line_read)))
	{
		//printf("in erase heredoc fn (4)\n");
		**line_read = ' ';
		(*line_read)++;
//		*(*line_read)++ = ' ';

	}
//	while (**line_read && is_whitespace(**line_read))
//		(*line_read)++;
	while (**line_read)
	{
		if ((*flag == 1 && **line_read == 39)
			|| (*flag == 2 && **line_read == '"'))
		{
			*flag = 0;
			**line_read = ' ';
		}
		else if (*flag == 1 || *flag == 2)
			**line_read = ' ';
		else if (**line_read == 39)
		{
			*flag = 1;
			**line_read = ' ';
		}
		else if (**line_read == '"')
		{
			*flag = 2;
			**line_read = ' ';
		}
		else if (!is_whitespace(**line_read))
			**line_read = ' ';
		else if (is_whitespace(**line_read))
			break ;
		(*line_read)++;
	}
}

void	check_for_heredoc(t_params *params, int i, char **line_read)
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
		params->cmd_arr[i].heredoc_rightmost = is_heredoc_rightmost(*line_read, delim_len);
		if (delim)
			ft_heredoc(params, i, delim, flag);
		flag = 0;
//		if (**line_read == 39 || **line_read == '"')
//		{
//			printf("----->>>(1) In here\n");
//			flag = 1;
//			**line_read = ' ';
//			(*line_read)++;
//		}
//		while (++j < delim_len)
//		{
//			printf("----->>>(2) In here\n");
//			printf("*line_read = %s\n", *line_read);
//			**line_read = ' ';
//			(*line_read)++;
//		}
		//while (**line_read && j < delim_len)
		// the real erase_heredoc_and_delim .c
		erase_heredoc_and_delim(line_read, &flag);
		//ERASE_HEREDOC_AND_DELIM();
		//printf("*line_read after erasing away chars: %s\n", *line_read);
//		if (!is_whitespace(**line_read))
//		if (**line_read)
//		{
//			if (**line_read == 39 || **line_read == '"')
//				**line_read = ' ';
//			else if (*(*line_read + 1) == 39 || *(*line_read + 1) == '"')
//				*(*line_read + 1) = ' ';
//		}
//		printf("2 -- *line_read after erasing away chars: %s\n", *line_read);
//		if (flag)
//			**line_read = ' ';
	}
	else
		(*line_read)++;
}

int	open_all_heredocs(t_params *params, char *line_read)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (*line_read)
	{
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
		if ((flag == 1 && *line_read == 39)
			|| (flag == 2 && *line_read == '"'))
			flag = 0;
		else if (flag == 1 || flag == 2)
			;
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '"')
			flag = 2;
		else if (*line_read == '<')
		{
			if (*(line_read + 1))
			{
				check_for_heredoc(params, i, &line_read);
				//printf("after check for heredoc: %s\n", line_read);
				//printf("which part of string we are at after heredoc fn: %s\n",
				//		line_read);
				continue ;
			}
		}
		else if (*line_read == '|')
			i++;
		line_read++;
	}
	return (0);
}
