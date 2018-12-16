/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_set.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 19:10:29 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/12 16:54:53 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"

t_sjson_error			in_set(t_sjson_env *e,
								char **set,
								size_t set_length,
								t_sjson_boolean update_pos)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (++i < set_length)
	{
		j = 0;
		while (set[i][j])
			++j;
		if (j <= e->slimit - e->pos)
		{
			j = 0;
			while (j + e->pos < e->slimit && set[i][j] != '\0'
					&& set[i][j] == e->src[e->pos + j])
				++j;
			if (set[i][j] != '\0')
				continue ;
			if (update_pos)
				e->pos += j;
			return (SJSON_ERROR_OK);
		}
	}
	return (SJSON_ERROR_THE_FUCK_IS_THAT);
}
