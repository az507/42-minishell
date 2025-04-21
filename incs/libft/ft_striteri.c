/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 13:43:53 by achak             #+#    #+#             */
/*   Updated: 2024/06/09 14:52:41 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

/*#include <stdio.h>

void	function(unsigned int i, char *s)
{
	printf("index = %d and char = %c\n", i, s[i]);
}

int	main(void)
{
	char *s = "jkhfihrihfiqhdiqhodqhohhodhoah2qpjdqojdqojodwj";
	void (*f)(unsigned int, char*) = &function;
	ft_striteri(s, f);
}*/
