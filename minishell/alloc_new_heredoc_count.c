/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_new_heredoc_count.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 09:50:41 by achak             #+#    #+#             */
/*   Updated: 2024/04/18 16:06:15 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*alloc_new_heredoc(char *str, t_params *params)
{
	int		str_len;
	char	*here_doc;

	str_len = count_new_heredoc_len(str, params);
	here_doc = malloc(sizeof(char) * (str_len + 1));
	if (here_doc)
	{
		here_doc[str_len] = '\0';
		copy_new_heredoc(str, here_doc, params);
	}
	free(str);
	return (here_doc);
}

void	count_heredoclen_for_var(char **str, int *count, t_params *params)
{
	int		i;
	t_env	*temp;

	i = 1;
	temp = *(params->head_env);
	if ((*str)[i])
	{
		while ((*str)[i] && (is_alphabet((*str)[i])
			|| is_numeric((*str)[i]) || (*str)[i] == '_'))
			i++;
	}
	while (temp)
	{
		if (!my_strncmp(*str, temp->key, i))
			break ;
		temp = temp->next;
	}
	if (temp)
		if (temp->value)
			*count += my_strlen(temp->value);
	*str += i - 1;
}
/*
//				i = 1;
//				if (str[i])
//				{
//					while (str[i] && (is_alphabet(str[i])
//						|| is_numeric(str[i]) || str[i] == '_'))
//						i++;
//				}
//				temp = *(params->head_env);
//				while (temp)
//				{
//					if (!my_strncmp(str, temp->key, i))
//						break ;
//					temp = temp->next;
//				}
//				if (temp)
//					if (temp->value)
//						count += my_strlen(temp->value);
//				str += i - 1;
*/

int	count_new_heredoc_len(char *str, t_params *params)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == '$' && *(str + 1))
		{
			str++;
			if (is_alphabet(*str) || *str == '_' || *str == '?')
				count_heredoclen_for_var(&str, &count, params);
			else
				count += 2;
		}
		else
			count++;
		str++;
	}
	return (count);
}

/*
void	ft_heredoc(t_params *params, int i, char *token_arr, int rv)
{
	static int	nbr = 0;
	char		*here_doc;
	char		*str;
	int			fd1;
	int			fd2;

	here_doc = strjoin_and_free_str("here_doc", ft_itoa(nbr++), 2);
	here_doc = strjoin_and_free_str("/tmp/", here_doc, 2);
	fd1 = open(here_doc, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd1 == -1)
		perror("open heredoc");
	else
	{
		str = get_next_line(STDIN_FILENO);
		while (str)
		{
			if (my_strcmp(token_arr, str))
			{
				free(str);
				fd2 = open(here_doc, O_RDONLY);
				close(fd1);
				*(params->cmd_arr[i].stdin_fds) = fd2;
				params->cmd_arr[i].here_doc = here_doc;
				return ;
			}
			if (look_for_env_var(str) && rv == 4)
				str = alloc_new_heredoc(str, params);
			write(fd1, str, my_strlen(str));
			free(str);
			str = get_next_line(STDIN_FILENO);
		}
		free(here_doc);
		close(fd1);
	}
}

void	heredoc_til_delimiter(t_command *cmd_struct, int fd1, char *token_arr,
		char *here_doc)
{
	char	*str;
	int		fd2;

	str = get_next_line(STDIN_FILENO);
	while (str)
	{
		if (my_strcmp(token_arr, str))
		{
			free(str);
			fd2 = open(here_doc, O_RDONLY);
			close(fd1);
			*(cmd_struct->stdin_fds) = fd2;
			cmd_struct->here_doc = here_doc;
			return ;
		}
		if (look_for_env_var(str))
			str = alloc_new_heredoc(str, params);
		write(fd1, str, my_strlen(str));
		free(str);
		str = get_next_line(STDIN_FILENO);
	}
	free(here_doc);
	close(fd1);
}

void	ft_heredoc(t_params *params, int i, char *token_arr)
{
	static int	nbr = 0;
	char		*here_doc;
	int			fd1;

	here_doc = strjoin_and_free_str("here_doc", ft_itoa(nbr++), 2);
	here_doc = strjoin_and_free_str("/tmp/", here_doc, 2);
	fd1 = open(here_doc, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd1 == -1)
		perror("open heredoc");
	else
		heredoc_til_delimiter(&params->cmd_arr[i], fd1, token_arr,
			here_doc);
}
*/
