/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 10:49:37 by achak             #+#    #+#             */
/*   Updated: 2024/06/15 11:40:38 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_sep(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		i;
	int		start;
	int		end;

	start = 0;
	i = 0;
	while (s1[i] != '\0' && check_sep(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && check_sep(s1[end - 1], set))
		end--;
	str = malloc(sizeof(char) * (end - start + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (start < end)
		str[i++] = s1[start++];
	str[i] = 0;
	return (str);
}

/*#include <stdio.h>

int	main(void)
{
	char	*str;
	char const *s1 = ",gfedcba.This is strictly for testing purposes,abcdeg";
	char const *set = ",.abcdefg";

	str = ft_strtrim(s1, set);
	printf("%s\n", str);
	free(str);
}*/
