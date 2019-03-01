/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_test_type.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 19:26:23 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:20:20 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"

t_sjson_error	sjson_test_type(const t_sjson_value *value,
								const t_sjson_value_type type)
{
	if (value == NULL || (!(value->type & type) && !(type & SJSON_TYPE_NONE))
			|| value->parent == NULL)
		return (0);
	if ((value->type & SJSON_TYPE_ARRAY) && value->data.ar.values == NULL)
		return (0);
	if ((value->type & SJSON_TYPE_OBJECT) && value->data.obj.nb_groups == 0)
		return (0);
	if ((value->type & SJSON_TYPE_STRING) && value->data.str == NULL)
		return (0);
	if (value->type & SJSON_TYPE_BOOLEAN && value->data.bol & ~1)
		return (0);
	return (1);
}
