/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 18:03:44 by achak             #+#    #+#             */
/*   Updated: 2023/09/18 12:17:05 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;

	ptr = NULL;
	if (c % 256 == '\0')
		return ((char *)s + ft_strlen(s));
	while (*s)
	{
		if (*s == c % 256)
			ptr = (char *)s;
		s++;
	}
	return (ptr);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	const char	*s;
	char	*b;
	char	*a;

	s = "teste";
	b = ft_strrchr(s, 101);
	a = strrchr(s, 101);
	printf("%s\n", b);
	printf("%s\n", a);
}*/
