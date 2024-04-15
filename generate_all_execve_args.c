/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_all_execve_args.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 16:06:56 by achak             #+#    #+#             */
/*   Updated: 2024/04/04 16:20:54 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	nbr_of_pipes(char *line_read)
{
	int	nbr;
	int	flag;

	nbr = 0;
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
		else if (flag == 0)
			if (*line_read == '|')
				nbr++;
		line_read++;
	}
	return (nbr);
}

int	count_cmd_words(char *temp)

char	**generate_execve_args(t_params *params, char **temp)
{
	int		cmd_words;
	char	**cmd_args;

	cmd_words = count_cmd_words(*temp);
	cmd_args = malloc(sizeof(char *) * (cmd_words + 1));
}

char	***generate_all_execve_args(t_params *params, char *line_read)
{
	int		cmd_nbr;
	char	***execve_args;
	char	*temp;

	cmd_nbr = nbr_of_pipes(line_read) + 1;
	execve_args = malloc(sizeof(char **) * (cmd_nbr + 1));
	temp = line_read;
	if (execve_args)
	{
		execve_args[cmd_nbr] = NULL;
		while (++i < cmd_nbr)
			execve_args[i] = generate_execve_args(params, &temp);
	}
	return (execve_args);
}
