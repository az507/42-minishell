/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 15:31:30 by achak             #+#    #+#             */
/*   Updated: 2024/06/10 20:16:07 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (s)
		while (s[i] != '\0')
			i++;
	return (i);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	printf("%zu\n", ft_strlen("This is a test."));
	printf("%zu\n", strlen("This is a test."));
	
}*/
