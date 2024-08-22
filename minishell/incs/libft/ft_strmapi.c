/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 13:08:01 by achak             #+#    #+#             */
/*   Updated: 2023/09/18 16:18:27 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	i;

	i = 0;
	str = ft_strdup(s);
	if (str == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		str[i] = (*f)(i, str[i]);
		i++;
	}
	return (str);
}

/*#include <stdio.h>

char	function(unsigned int i, char str)
{
 	printf("index = %d and char = %c\n", i, str);
 	return str - 32;
}

int	main()
{
 	char str[10] = "hello.";
 	printf("The result is %s\n", str);
	char (*f)(unsigned int, char) = &function;
 	char *result = ft_strmapi(str, f);
 	printf("The result is %s\n", result);
 	return 0;
}*/
