/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 12:00:13 by achak             #+#    #+#             */
/*   Updated: 2023/09/19 10:54:17 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	int		i;

	i = 0;
	while (s[i] != '\0')
		i++;
	str = malloc(sizeof(char) * (i + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*#include <stdio.h>
#include <string.h>

int	main(void)
{
	char 	*str;
	char	*str2;

	str = ft_strdup("gjiosioshfgoa");
	str2 = strdup("gjiosioshfgoa");
	printf("ft_strdup: %s\n", str);
	printf("strdup: %s\n", str2);
	free(str);
	free(str2);
}*/
