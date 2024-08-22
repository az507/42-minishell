/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:19:43 by achak             #+#    #+#             */
/*   Updated: 2023/09/13 10:28:10 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (*(unsigned char *)s1 == *(unsigned char *)s2 && n-- > 1)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	const void *s1 = "aaaaab";
	const void *s2 = "aaaaaz";

	printf("ft_memcmp: %d\n", ft_memcmp(s1, s2, 6));
	printf("memcmp: %d\n", memcmp(s1, s2, 6));
}*/
