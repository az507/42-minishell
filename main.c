/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 11:36:43 by achak             #+#    #+#             */
/*   Updated: 2024/03/25 11:42:01 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	main(void)
{
	static char	*line_read;

	while (1)
	{
		if (line_read)
		{
			free(line_read);
			line_read = NULL;
		}
		line_read = readline("$> ");
		if (line_read)
		{
			char	*temp = line_read;
			printf("strlen(temp) = %zu\n", strlen(temp));
//			while (temp && *temp)
//			{
//				printf("*temp = %c\n", *temp++);
//				usleep(50000);
//			}
		}
	}
}
