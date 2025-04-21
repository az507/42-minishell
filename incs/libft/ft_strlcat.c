/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 13:40:13 by achak             #+#    #+#             */
/*   Updated: 2024/06/09 14:53:25 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i1;
	size_t	i2;
	size_t	b;

	i1 = 0;
	i2 = 0;
	b = 0;
	while (dest[i1] != '\0')
		i1++;
	while (src[b] != '\0')
		b++;
	if (size <= i1)
		b = b + size;
	else
		b = b + i1;
	while (src[i2] != '\0' && size > (i1 + 1))
	{
		dest[i1] = src[i2];
		i1++;
		i2++;
	}
	dest[i1] = '\0';
	return (b);
}

/*size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	src_length;
	size_t	dst_length;
	size_t	i;

	src_length = 0;
	dst_length = 0;
	i = 0;
	while (dst[dst_length] != '\0')
		dst_length++;
	while (src[src_length] != '\0')
		src_length++;
	while (src[i] != '\0' && size > (dst_length + 1))
		dst[dst_length++] = src[i++];
	dst[dst_length] = '\0';
	if (dst_length >= size)
		dst_length = size;
	return (src_length + dst_length);
}*/

/*#include <stdio.h>
#include <bsd/string.h>

int	main(void)
{
	char dst[50] = "This is a ";
	const char *src = "test for testing purposes";

	printf("before ft_strlcat: %s\n", dst);
	printf("return value-ft_strlcat=%zu\n", ft_strlcat(dst, src, 25));
	printf("after ft_strlcat: %s\n", dst);
	printf("return value-strlcat=%zu\n", strlcat(dst, src, 25));
	printf("after strlcat: %s\n", dst);
}*/

/*#include <stdio.h>
#include <bsd/string.h>

int	main(void)
{
	char	dst[50] = "pqrstuvwxyz";
	const char *src = "abcd";

	printf("%zu\n", ft_strlcat(dst, src, 20));
	printf("%s\n", dst);
	printf("%zu\n", strlcat(dst, src, 20));
	printf("%s\n", dst);
}*/
