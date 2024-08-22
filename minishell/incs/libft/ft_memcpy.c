/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 16:19:50 by achak             #+#    #+#             */
/*   Updated: 2024/06/10 17:50:30 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;
	size_t		i;

	d = (char *)dest;
	s = (const char *)src;
	i = 0;
	if (d == 0 && s == 0)
		return (0);
	while (i < n && s[i])
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	
	const void	*src;
	void	*dest;
	char	c[50] = "ABC";

	src = "This is a test.";
	dest = &c;
	printf("before ft_memcpy: %s\n", (char *)dest);
	ft_memcpy(dest, src, 4);
	printf("after ft_memcpy: %s\n", (char *)dest);
	memcpy(dest, src, 10);
	printf("after memcpy: %s\n", (char *)dest);
	printf("%s\n", (char *)ft_memcpy(((void *)0), ((void *)0), 3));
}*/
