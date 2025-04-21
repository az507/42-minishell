/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:24:37 by achak             #+#    #+#             */
/*   Updated: 2023/09/19 10:19:12 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	d = dest;
	s = src;
	if (d == 0 && s == 0)
		return (0);
	if (d < s)
		while (n--)
			*d++ = *s++;
	else
	{
		while (n--)
			d[n] = s[n];
	}
	return (dest);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	void	*dest;
	char	d[50] = "abcdefghijk";

	dest = &d;
	printf("before ft_memmove: %s\n", (char *) dest);
	dest = ft_memmove(d, "This is a p", 11);
	printf("after ft_memmove: %s\n", (char *)dest);
	dest = memmove(d, "fhiuehgfuie", 12);
	printf("after memmove: %s\n", (char *)dest);
}*/
