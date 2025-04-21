/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:26:09 by achak             #+#    #+#             */
/*   Updated: 2023/09/20 10:34:35 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	ptr = malloc(nmemb * size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, size * nmemb);
	return (ptr);
}

/*#include <stdio.h>

int	main(void)
{
	int	*a;
	int	*b;
	size_t	nmemb;

	nmemb = 10;
	a = (int *)ft_calloc(nmemb, 8);
	b = (int *)calloc(nmemb, 8);
	while (nmemb--)
	{
		printf("%d\n", a[nmemb]);
		printf("%p\n", &a[nmemb]);
		printf("%d\n", b[nmemb]);
		printf("%p\n\n", &b[nmemb]);
	}
	printf("ft_calloc-0th element: %d\n", *(int *)ft_calloc(5, 1));
	printf("ft_calloc-1st element: %d\n", *(int *)(ft_calloc(5, 1) + 1));
	printf("ft_calloc-2nd element: %d\n", *(int *)(ft_calloc(5, 1) + 2));
	printf("ft_calloc-3rd element: %d\n", *(int *)(ft_calloc(5, 1) + 3));
	printf("ft_calloc-4th element: %d\n\n", *(int *)(ft_calloc(5, 1) + 4));
	printf("calloc-0th element: %d\n", *(int *)calloc(5, 1));
	printf("calloc-1st element: %d\n", *(int *)(calloc(5, 1) + 1));
	printf("calloc-2nd element: %d\n", *(int *)(calloc(5, 1) + 2));
	printf("calloc-3rd element: %d\n", *(int *)(calloc(5, 1) + 3));
	printf("calloc-4th element: %d\n", *(int *)(calloc(5, 1) + 4));
	printf("a[0]: %d\n", *a);
	printf("a[1]: %d\n", *(a + 1));
	printf("a[2]: %d\n", *(a + 2));
	printf("a[3]: %d\n", *(a + 3));
	printf("a[4]: %d\n\n", *(a + 4));
	printf("b[0]: %d\n", *b);
	printf("b[1]: %d\n", *(b + 1));
	printf("b[2]: %d\n", *(b + 2));
	printf("b[3]: %d\n", *(b + 3));
	printf("b[4]: %d\n", *(b + 4));
	free(a);
	free(b);
}*/
