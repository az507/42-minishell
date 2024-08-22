/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 15:41:12 by achak             #+#    #+#             */
/*   Updated: 2024/06/10 15:35:43 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;
	size_t			i;

	i = 0;
	str = s;
	while (i < n)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	char str[50] = "fkheuifahwfeiroqjofqoqjwoqqdqwojdqo";
	printf("\nBefore ft_memset: %s\n", str);
	ft_memset(str + 5, '.', 8);
	printf("\nAfter ft_memset: %s\n", str);
	memset(str + 13, '!', 7);
	printf("\nAfter memset: %s\n", str);
}*/
