/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/01 00:00:00 by hmartzol          #+#    #+#             */
/*   Updated: 2019/01/01 00:00:00 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*sstrchr(char *str, int c)
{
	while (*str != '\0' && *str != c)
		++str;
	return (*str == c ? str : NULL);
}

int		sstrncmp(char *str1, char *str2, size_t n)
{
	while (n-- && *str1 != '\0' && *str2 != '\0' && *str1 == *str2)
	{
		++str1;
		++str2;
	}
	return (*str1 - *str2);
}

int		sstrcmp(char *str1, char *str2)
{
	while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2)
	{
		++str1;
		++str2;
	}
	return (*str1 - *str2);
}

char	*sstrdup(char *str)
{
	size_t	s;
	char	*out;

	s = 0;
	while (str[s] != '\0')
		++s;
	if ((out = malloc((s + 1) * sizeof(char))) == NULL)
		return (NULL);
	out[s] = '\0';
	while (s--)
		out[s] = str[s];
	return (out);
}

size_t	sstrlen(char *str)
{
	size_t	l;

	l = 0;
	while (str[l] != '\0')
		++l;
	return (l);
}
