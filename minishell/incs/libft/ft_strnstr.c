/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 12:25:03 by achak             #+#    #+#             */
/*   Updated: 2023/09/18 14:27:58 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	len2;

	if (*little == '\0')
		return ((char *)big);
	while (*big != '\0' && len--)
	{
		i = 0;
		if (*big == little[i])
		{
			len2 = len;
			while (big[i] == little[i] && little[i] != '\0' && len2-- + 1 > 0)
				i++;
			if (little[i] == '\0')
				return ((char *)big);
		}
		big++;
	}
	return (0);
}

/*#include <stdio.h>
#include <bsd/string.h>

int	main(void)
{
	const char *big = "aaxx";
	const char *little = "xx";

	printf("ft_strnstr: %s\n", ft_strnstr(big, little, 4));
	printf("strnstr: %s\n", strnstr(big, little, 4));
}*/
