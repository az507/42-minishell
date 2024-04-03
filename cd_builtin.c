/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 10:41:27 by achak             #+#    #+#             */
/*   Updated: 2024/04/03 18:52:21 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

//int	free_cd_strings(char **dir_arr, int flag)
int	free_cd_strings(char *oldpwd, char *pwd, char **dir_arr, int flag)
{
	free(oldpwd);
	if (pwd)
		free(pwd);
	if (dir_arr)
		free_array(dir_arr);
	return (flag);
}

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
	}
	free(str);
	return (new_str);
}

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

int	cd_wrapper(t_env **head_env, char *new_path, char *path, char **dir_arr)
{
	char	*oldpwd;
	char	*pwd;
	char	*ch_path;
	int		flag;

	pwd = NULL;
	flag = 0;
//	oldpwd = strjoin_and_free_str("OLDPWD=", getcwd(NULL, 200), 2);
	oldpwd = find_env_var_value(*head_env, "PWD");
	oldpwd = strjoin_and_free_str("OLDPWD=", oldpwd, 0);
	ch_path = path;
	printf("ch_path = %s\n", ch_path);
	if (new_path)
		ch_path = new_path;
	printf("new_path = %s\n", new_path);
	if (chdir(ch_path) == -1)
	{
		flag = 1;
		ft_dprintf(STDERR_FILENO, "cd: %s: %s\n", path,
			strerror(errno));
	}
	else
	{
//		pwd = strjoin_and_free_str("PWD=", getcwd(NULL, 200), 2);
		pwd = strjoin_and_free_str("PWD=", ch_path, 0);
		if (check_dir_path_has_c(pwd, '.'))
		{
			free(pwd);
			pwd = getcwd(NULL, 200);
		}
		if (my_strlen(pwd) > 4)
			if (pwd[my_strlen(pwd) - 1] == '\\')
				pwd = remove_last_bytes_from_str(pwd, 1);
		update_existing_entry(oldpwd, head_env);
		update_existing_entry(pwd, head_env);
	}
	//return (free_cd_strings(dir_arr, flag));
	return (free_cd_strings(oldpwd, pwd, dir_arr, flag));
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
		cdpath = find_env_var_value(*head_env, "CDPATH");
		if (!cdpath)
			cdpath = getcwd(NULL, 200);
		else
			dir_arr = ft_split(cdpath, ':');
		return (different_cd_criteria(head_env, cmd_args, cdpath, dir_arr));
	}
}
