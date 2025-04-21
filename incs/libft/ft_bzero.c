/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 15:52:06 by achak             #+#    #+#             */
/*   Updated: 2023/09/13 11:46:18 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*str;
	size_t	i;

	str = s;
	i = 0;
	while (i < n)
	{
		str[i] = '\0';
		i++;
	}
}

/*#include <stdio.h>
#include <strings.h>

int	main(void)
{
	char	c[50] = "abcdef";
	void	*s;

	s = &c;
	printf("before bzero: %s\n", (char *)s);
	ft_bzero(s, 3);
	printf("after ft_bzero[0th char]: %c\n", *(char *)s);
	printf("after ft_bzero[1st char]: %c\n", *(char *)(s + 1));
	printf("after ft_bzero[2nd char]: %c\n", *(char *)(s + 2));
	printf("after ft_bzero[3rd char]: %c\n", *(char *)(s + 3));
	printf("after ft_bzero[4th char]: %c\n", *(char *)(s + 4));
	printf("after ft_bzero[5th char]: %c\n", *(char *)(s + 5));
	bzero(s, 5);
	printf("after bzero[5th char]: %c\n", *(char *)s);
	printf("after bzero[5th char]: %c\n", *(char *)(s + 1));
	printf("after bzero[5th char]: %c\n", *(char *)(s + 2));
	printf("after bzero[5th char]: %c\n", *(char *)(s + 3));
	printf("after bzero[5th char]: %c\n", *(char *)(s + 4));
	printf("after bzero[5th char]: %c\n", *(char *)(s + 5));	
}*/
