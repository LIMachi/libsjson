/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 17:18:51 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 20:59:48 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sjson.h"
#include "sjson_functions.h"

void	sjson_free(t_sjson_value *value)
{
	size_t	i;

	if (value == NULL)
		return ;
	if (value->type == SJSON_TYPE_STRING)
		free(value->data.str);
	else if (value->type == SJSON_TYPE_ARRAY)
	{
		i = -1;
		while (++i < value->data.ar.nb_values)
			sjson_free(value->data.ar.values[i]);
		free(value->data.ar.values);
	}
	else if (value->type == SJSON_TYPE_OBJECT)
	{
		i = -1;
		while (++i < value->data.obj.nb_pairs)
		{
			free(value->data.obj.pairs[i]->key);
			sjson_free(value->data.obj.pairs[i]->value);
			free(value->data.obj.pairs[i]);
		}
		free(value->data.obj.pairs);
	}
	free(value);
}
