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
#include <libft.h>

t_sjson_error	sjson_search_pair_in_object(t_sjson_value *v,
											const char *str,
											t_sjson_value **out)
{
	unsigned long	i;
	t_sjson_object	*obj;
	t_sjson_error	error;
	size_t			len;

	len = 0;
	while (str[len] != '\0')
		++len;
	if ((error = sjson_test_type(v, SJSON_TYPE_OBJECT)) != SJSON_ERROR_OK)
		return (error);
	obj = &v->data.obj;
	i = -1;
	while (++i < obj->nb_pairs)
		if (obj->pairs[i] != NULL && obj->pairs[i]->key != NULL
				&& len == obj->pairs[i]->key->length
				&& !ft_strncmp(obj->pairs[i]->key->data, (char*)str, len))
		{
			*out = obj->pairs[i]->value;
			return (SJSON_ERROR_OK);
		}
	return (SJSON_ERROR_TARGET_NOT_FOUND);
}
