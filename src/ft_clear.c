/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_search_pair_in_object.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 19:34:23 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:54:42 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

void	*ft_clear(char *form, ...)
{
	va_list	va;
	void	*out;

	out = NULL;
	if (form == NULL)
		return (out);
	va_start(va, form);
	while (*form != '\0')
	{
		if (*form == 'd')
			close(va_arg(va, int));
		else if (*form == 'p')
			free(va_arg(va, void *));
		else if (*form == 'r')
			out = va_arg(va, void *);
		++form;
	}
	va_end(va);
	return (out);
}