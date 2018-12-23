/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_search_index_in_array.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 18:54:29 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:54:21 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"

t_sjson_error	sjson_search_index_in_array(t_sjson_value *value,
											size_t index,
											t_sjson_value **out)
{
	if (!sjson_test_type(value, SJSON_TYPE_ARRAY))
		return (SJSON_ERROR_MISMATCHED_NODE_TYPE);
	if (index >= value->data.ar.nb_values)
		return (SJSON_ERROR_TARGET_NOT_FOUND);
	*out = value->data.ar.values[index];
	return (SJSON_ERROR_OK);
}
