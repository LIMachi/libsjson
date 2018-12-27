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
	size_t			i;
	char			*key;
	t_sjson_value	*tmp;

	if (value == NULL)
		return ;
	if (value->type == SJSON_TYPE_STRING)
	{
		free(value->data.str);
		value->data.str = NULL;
	}
	else if (value->type == SJSON_TYPE_ARRAY)
	{
		i = -1;
		while (++i < value->data.ar.nb_values)
			sjson_free(value->data.ar.values[i]);
		free(value->data.ar.values);
		value->data.ar.values = NULL;
	}
	else if (value->type == SJSON_TYPE_OBJECT)
	{
		i = 0;
		while (ft_swiss_table_iterate(&value->data.obj, &i, (void**)&key, (void**)&tmp))
		{
			free(key);
			sjson_free(tmp);
		}
		ft_swiss_table_destroy(&value->data.obj);
/*		i = -1;
		while (++i < value->data.obj.nb_pairs)
		{
			free(value->data.obj.pairs[i]->key);
			value->data.obj.pairs[i]->key = NULL;
			sjson_free(value->data.obj.pairs[i]->value);
			value->data.obj.pairs[i]->value = NULL;
			free(value->data.obj.pairs[i]);
			value->data.obj.pairs[i] = NULL;
		}
		free(value->data.obj.pairs);
		value->data.obj.pairs = NULL;*/
	}
	value->parent = NULL;
	value->type = SJSON_TYPE_INVALID;
	free(value);
}
