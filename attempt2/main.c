/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:15:53 by achak             #+#    #+#             */
/*   Updated: 2024/04/19 10:51:47 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	split_raw_str_by_pipes(t_params *params, char *line_read)
{
	int		str_len;
	int		i;
	char	*temp;

	i = -1;
	temp = line_read;
	while (++i < params->cmd_nbr)
	{
		str_len = count_len_til_pipe(temp);
		params->cmd_arr[i].raw_str = malloc(sizeof(char) * (str_len + 1));
		if (!params->cmd_arr[i].raw_str)
			return (-1);
		params->cmd_arr[i].raw_str[str_len] = '\0';
		copy_line_til_pipe(params, i, &temp);
		if (*temp == '|')
			temp++;
		while (is_whitespace(*temp))
			temp++;
	}
	return (0);
}
/*		^^^^ if raw str malloc fails 
//			while (--i >= 0)
//				free(params->cmd_arr[i].raw_str);
			//cleanup_params(params);
*/

void	main_shell_loop(t_params *params, char *line_read)
{
	int			cmd_nbr;
	t_command	*cmd_arr;
	char		*temp;

	temp = line_read;
	add_history(line_read);
	cmd_nbr = nbr_of_pipes(temp) + 1;
	if (cmd_nbr == -1 || check_redir_syntax(temp) == -1
		|| check_empty_line(temp) == -1)
		return ;
	cmd_arr = malloc(sizeof(t_command) * cmd_nbr);
	if (!cmd_arr)
		return ;
	init_cmd_arr(params, cmd_arr, cmd_nbr, line_read);
	if (open_all_heredocs(params, line_read) == -1)
		return ;
	if (split_raw_str_by_pipes(params, line_read) == -1)
		return ;
	preparing_fork_and_execve(params, line_read);
}

int	main(int ac, char **av, char **env)
{
	static char	*line_read;
	t_params	params;
	t_env		*head_env;
	char		*cwd;
	int			flag;

	flag = 0;
	head_env = create_symbol_table(ac, av, env);
	init_params(&params, &head_env);
	set_up_signals();
	while (1)
	{
		check_read_line_eof(line_read, flag, &params);
		cwd = strjoin_and_free_str(getcwd(NULL, 200), "$ ", 1);
		line_read = readline(cwd);
		free(cwd);
		if (line_read && *line_read)
			main_shell_loop(&params, line_read);
		flag = 1;
	}
}
