/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 14:32:48 by achak             #+#    #+#             */
/*   Updated: 2023/09/14 14:36:23 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	while (*s != '\0')
		write(fd, &(*s++), 1);
	write(fd, "\n", 1);
}

/*int	main(void)
{
	char *s = "FIHAIFAIKADOAJJ-KJJ0";
	ft_putendl_fd(s, 1);
}*/
