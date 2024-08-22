/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:10:42 by achak             #+#    #+#             */
/*   Updated: 2023/09/12 15:33:36 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}

/*#include <stdio.h>
#include <ctype.h>

int	main(void)
{
	int	c;
	int	d;

	c = 97;
	d = 96;
	printf("uppercase of %c is %c\n", c, ft_toupper(c));	
	printf("uppercase of %c is %c\n", c, toupper(c));
	printf("uppercase of %c is %c\n", d, ft_toupper(d));
	printf("uppercase of %c is %c\n", d, toupper(d));
}*/
