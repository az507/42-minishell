/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 14:27:08 by achak             #+#    #+#             */
/*   Updated: 2023/09/14 14:31:49 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	while (*s != '\0')
		write(fd, &(*s++), 1);
}

/*int	main(void)
{
	char *s = "abfiafiwafiahciahiwidhia";
	ft_putstr_fd(s, 1);
}*/
