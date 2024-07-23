/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_envp_arr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:36:58 by achak             #+#    #+#             */
/*   Updated: 2024/04/18 12:30:51 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	count_env_nbr(t_env *head_env)
{
	int	env_nbr;

	env_nbr = 0;
	while (head_env)
	{
		env_nbr++;
		head_env = head_env->next;
	}
	return (env_nbr);
}

int	copy_strs_to_envp_arr(int i, char **envp, t_env *temp)
{
	int	j;
	int	k;
	int	env_len;

	j = -1;
	k = -1;
	env_len = my_strlen(temp->key) + my_strlen(temp->value);
	envp[i] = malloc(sizeof(char) * (env_len + 2));
	if (!envp[i])
	{
		while (--i >= 0)
			free(envp[i]);
		free(envp);
		return (-1);
	}
	envp[i][env_len + 1] = '\0';
	while (++k < my_strlen(temp->key))
		envp[i][++j] = temp->key[k];
	envp[i][++j] = '=';
	k = -1;
	while (++k < my_strlen(temp->value))
		envp[i][++j] = temp->value[k];
	return (0);
}

char	**create_envp_arr(t_env *head_env)
{
	int		i;
	int		env_nbr;
	t_env	*temp;
	char	**envp;

	i = -1;
	env_nbr = count_env_nbr(head_env);
	temp = head_env;
	if (!env_nbr)
		return (NULL);
	envp = malloc(sizeof(char *) * (env_nbr + 1));
	if (envp)
	{
		envp[env_nbr] = NULL;
		while (++i < env_nbr && temp)
		{
			if (copy_strs_to_envp_arr(i, envp, temp) == -1)
				return (NULL);
			temp = temp->next;
		}
	}
	return (envp);
}
