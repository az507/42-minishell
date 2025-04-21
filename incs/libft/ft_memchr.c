/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 18:31:28 by achak             #+#    #+#             */
/*   Updated: 2023/09/19 10:21:12 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	while (n--)
	{
		if (*(unsigned char *)s == (unsigned char)c % 256)
			return ((void *)s);
		s++;
	}
	return (0);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	const void	*s;

	s = "pppppppppppppppppppppppppppppppppaa";
	printf("%s\n", (char *)ft_memchr(s, 'a' + 256, 34));
	printf("%s\n", (char *)memchr(s, 'a' + 256, 34));
}*/
