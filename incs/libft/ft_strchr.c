/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 17:25:10 by achak             #+#    #+#             */
/*   Updated: 2023/09/18 11:44:28 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*b;

	b = (char *)s;
	while (*b != c % 256)
	{
		if (!*b)
			return (0);
		b++;
	}
	return (b);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	const char	*s;
	char	*b;
//	char	*a;

	s = "teste";
	b = ft_strchr(s, 'e' + 256);
//	a = strchr(s, 'e' + 256);
	printf("%p\n", s);
	printf("%s\n", b);
	printf("%p\n", b);
//	printf("%s\n", a);
//	printf("%p\n", a);
//	s = "abcdefghijklmnopqrstuvwxyza";
//	printf("%s\n", ft_strchr(s, 122));
//	printf("%s\n", strchr(s, 122));
	
}*/
