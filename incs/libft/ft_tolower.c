/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:18:38 by achak             #+#    #+#             */
/*   Updated: 2023/09/12 15:31:33 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}

/*#include <stdio.h>
#include <ctype.h>

int	main(void)
{
	int	c;
	int	d;

	c = 70;
	d = 34;
	printf("lowercase of %c is %c\n", c, ft_tolower(c));
	printf("lowercase of %c is %c\n", c, tolower(c));
	printf("lowercase of %c is %c\n", d, ft_tolower(d));
	printf("lowercase of %c is %c\n", d, tolower(d));
}*/
