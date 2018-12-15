/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcpy.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 19:20:38 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:23:30 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*std_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = -1;
	while (++i < n)
		((char*)dest)[i] = ((char*)src)[i];
	return (dest);
}