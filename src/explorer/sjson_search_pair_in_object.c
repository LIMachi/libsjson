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

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"

t_sjson_error	sjson_search_pair_in_object(t_sjson_value *v,
											const char *str,
											t_sjson_value **out)
{
	if (!sjson_test_type(v, SJSON_TYPE_OBJECT))
		return (SJSON_ERROR_MISMATCHED_NODE_TYPE);
	if ((*out = ft_swiss_table_find(&v->data.obj, (void*)str, NULL)) == NULL)
		return (SJSON_ERROR_TARGET_NOT_FOUND);
	return (SJSON_ERROR_OK);
}
