/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 10:41:27 by achak             #+#    #+#             */
/*   Updated: 2024/04/23 13:53:27 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
/*
char	*remove_last_bytes_from_str(char *str, int n)
{
	int		str_len;
	int		i;
	char	*new_str;

	str_len = my_strlen(str) - n;
	i = -1;
	new_str = malloc(sizeof(char) * (str_len + 1));
	if (new_str)
	{
		new_str[str_len] = '\0';
		while (++i < n)
			new_str[i] = str[i];
	}	create_new_entry(pwd, head_env);
cd_builtin.c:			create_new_entry(oldpwd, hea
	free(str);
	return (new_str);
}
*/

int	check_dir_path_has_c(char *dir_path, char c)
{
	while (*dir_path)
	{
		if (*dir_path == c)
			return (1);
		dir_path++;
	}
	return (0);
}

void	cd_wrapper2(t_env **head_env, char *oldpwd, char *ch_path)
{
	char	*pwd;

	pwd = strjoin_and_free_str("PWD=", ch_path, 0);
	if (check_dir_path_has_c(pwd, '.'))
	{
		free(pwd);
		pwd = getcwd(NULL, 200);
		pwd = strjoin_and_free_str("PWD=", pwd, 2);
	}
	if (head_env)
	{
		if (!check_if_entry_exists(pwd, *head_env))
			create_new_entry(pwd, head_env);
		else
			update_existing_entry(pwd, head_env);
		if (!check_if_entry_exists(oldpwd, *head_env))
			create_new_entry(oldpwd, head_env);
		else
			update_existing_entry(oldpwd, head_env);
	}
	free(pwd);
}
/*
//		if (my_strlen(pwd) > 4)
//			if (pwd[my_strlen(pwd) - 1] == '\\')
//				pwd = remove_last_bytes_from_str(pwd, 1);
*/

int	cd_wrapper(t_env **head_env, char *new_path, char *path, char **dir_arr)
{
	char		*oldpwd;
	char		*ch_path;
	int			flag;
	struct stat	stat_buf;

	flag = 0;
	oldpwd = NULL;
	if (head_env)
	{
		oldpwd = find_env_var_value(*head_env, "PWD");
		oldpwd = strjoin_and_free_str("OLDPWD=", oldpwd, 0);
	}
	ch_path = path;
	if (new_path)
		ch_path = new_path;
	if (chdir(ch_path) == -1)
	{
		stat(path, &stat_buf);
		perror(path);
		flag = 1;
	}
	else
		cd_wrapper2(head_env, oldpwd, ch_path);
	return (free_cd_strings(oldpwd, dir_arr, flag));
}

int	different_cd_criteria(t_env **head_env, char **cmd_args, char *cdpath,
		char **dir_arr)
{
	char	*new_path;
	int		rv;

	new_path = NULL;
	rv = 0;
	if (cmd_args[1][0] == '/')
	{
		if (!dir_arr)
			free(cdpath);
		return (cd_wrapper(head_env, NULL, cmd_args[1], dir_arr));
	}
	else if (!dir_arr)
	{
		new_path = strjoin_and_free_str("/", cmd_args[1], 0);
		new_path = strjoin_and_free_str(cdpath, new_path, 2);
		free(cdpath);
		rv = cd_wrapper(head_env, new_path, cmd_args[1], NULL);
		free(new_path);
	}
	else
		return (search_dir_arr(head_env, dir_arr, cmd_args[1]));
	return (rv);
}

int	cd_builtin(t_env **head_env, char **cmd_args)
{
	char	*cdpath;
	char	**dir_arr;

	cdpath = NULL;
	dir_arr = NULL;
	if (!cmd_args[1])
	{
		write(STDERR_FILENO, "cd: pathname needed\n", 20);
		return (1);
	}
	else
	{
		if (cmd_args[2])
		{
			write(STDERR_FILENO, "cd: too many arguments\n", 23);
			return (1);
		}
		if (head_env)
			cdpath = find_env_var_value(*head_env, "CDPATH");
		if (!cdpath)
			cdpath = getcwd(NULL, 200);
		else
			dir_arr = ft_split(cdpath, ':');
		return (different_cd_criteria(head_env, cmd_args, cdpath, dir_arr));
	}
}
