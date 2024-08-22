/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 13:03:10 by achak             #+#    #+#             */
/*   Updated: 2023/09/15 15:33:37 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	length;

	i = 0;
	length = 0;
	while (src[length] != '\0')
		length++;
	if (size < 1)
		return (length);
	while (i < size - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (length);
}

/*#include <stdio.h>
#include <bsd/string.h>

int	main(void)
{
	char	dst[50] = "abcdefghijklmnop";

	printf("old dst[50]: %s\n", dst);
	printf("using ft_strlcpy\n");
	printf("length of src: %zu\n", ft_strlcpy(dst, "FHAIHWEAHFOE", 13));
	printf("new dst[50]: %s\n", dst);
	printf("dst[12]: %c\n", dst[12]);
	printf("dst[13]: %c\n", dst[13]);
	printf("dst[14]: %c\n", dst[14]);	
	printf("dst[15]: %c\n", dst[15]);
	printf("using strlcpy\n");
	printf("length of src: %zu\n", strlcpy(dst, "ZZZZZZZZZZZZ", 13));
        printf("new dst[50]: %s\n", dst);
        printf("dst[12]: %c\n", dst[12]);
        printf("dst[13]: %c\n", dst[13]);
        printf("dst[14]: %c\n", dst[14]);    
        printf("dst[15]: %c\n", dst[15]);	
}*/
