/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 10:47:53 by achak             #+#    #+#             */
/*   Updated: 2024/04/22 16:30:29 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	perror_filename(char *filename, int open_mode)
{
    (void)filename;
	if (open_mode == 1)
		filename = ">>";
	else if (open_mode == 2)
		filename = ">";
	else if (open_mode == 3)
		filename = "<";
	access("", F_OK | X_OK);
	perror(NULL);
	return (-1);
}

int	open_new_files(t_params *params, int i, int open_mode, char *filename)
{
	int	stdin_fd;

	stdin_fd = -2;
	if (open_mode == 2)
		params->cmd_arr[i].stdout_fd = open(filename,
				O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (open_mode == 1)
		params->cmd_arr[i].stdout_fd = open(filename,
				O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (open_mode == 3)
		stdin_fd = open(filename, O_RDONLY);
	if (stdin_fd == -1 || params->cmd_arr[i].stdout_fd == -1)
	{
		perror(filename);
		free(filename);
		return (-1);
	}
	if (params->cmd_arr[i].heredoc_rightmost != 1 && stdin_fd != -2)
		params->cmd_arr[i].stdin_fd = stdin_fd;
	else if (stdin_fd != -2)
		close(stdin_fd);
	return (0);
}

int	settle_redirects(t_params *params, int i, char **temp)
{
	int		open_mode;
	int		flag;
	char	*filename;

	open_mode = check_type_of_redirect(*temp);
	flag = 0;
	filename = expand_filename(params, *temp);
	if (!filename)
		return (perror_filename(filename, open_mode));
	close_existing_fds(params, i, open_mode);
	if (open_new_files(params, i, open_mode, filename) == -1)
		return (-1);
	while (**temp && (**temp == '<' || **temp == '>' || is_whitespace(**temp)))
	{
		**temp = ' ';
		(*temp)++;
	}
	erase_redirs_from_str(temp, &flag);
	free(filename);
	if (flag != 3)
		return (-2);
	return (0);
}

void	skip_quoted_chars(char *temp, int *flag)
{
	if ((*flag == 1 && *temp == 39)
		|| (*flag == 2 && *temp == '"'))
		*flag = 0;
	else if (*flag == 1 || *flag == 2)
		;
	else if (*temp == 39)
		*flag = 1;
	else if (*temp == '"')
		*flag = 2;
}

int	open_redirects(t_params *params, int i)
{
	int		flag;
	int		rv;
	char	*temp;

	flag = 0;
	rv = 0;
	temp = params->cmd_arr[i].raw_str;
	while (*temp)
	{
		if (flag != 0 || *temp == 39 || *temp == '"')
			skip_quoted_chars(temp, &flag);
		else if (*temp == '<' || *temp == '>')
		{
			rv = settle_redirects(params, i, &temp);
			if (rv == -1 || rv == -2)
				return (rv);
			continue ;
		}
		temp++;
	}
	return (0);
}
