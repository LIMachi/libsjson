/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/29 17:02:02 by hmartzol          #+#    #+#             */
/*   Updated: 2018/12/15 18:10:27 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"
#include <libft.h>

static inline t_sjson_value_type	evaluate_type(t_sjson_env *e,
												t_sjson_value *out)
{
	if (in_set(e, SJSON_OBJECT_STARTERS, SJSON_OBJECT_COUNT, 1)
			== SJSON_ERROR_OK)
		return (out->type = SJSON_TYPE_OBJECT);
	if (in_set(e, SJSON_ARRAY_STARTERS, SJSON_ARRAY_COUNT, 1) == SJSON_ERROR_OK)
		return (out->type = SJSON_TYPE_ARRAY);
	if (in_set(e, SJSON_NULL_SET, SJSON_NULL_COUNT, 1) == SJSON_ERROR_OK)
		return (out->type = SJSON_TYPE_NULL);
	if (in_set(e, SJSON_TRUE_SET, SJSON_TRUE_COUNT, 1) == SJSON_ERROR_OK)
	{
		out->data.bol = 1;
		return (out->type = SJSON_TYPE_BOOLEAN);
	}
	if (in_set(e, SJSON_FALSE_SET, SJSON_FALSE_COUNT, 1) == SJSON_ERROR_OK)
	{
		out->data.bol = 0;
		return (out->type = SJSON_TYPE_BOOLEAN);
	}
	if (e->src[e->pos] == '-' || (SJSON_EXTEND && e->src[e->pos] == '+')
			|| (ft_isdigit(e->src[e->pos])
				&& (SJSON_EXTEND || e->src[e->pos] != '0'))
			|| (SJSON_EXTEND && e->src[e->pos] == '.'))
		return (out->type = SJSON_TYPE_REAL);
	return (out->type = SJSON_TYPE_INVALID);
}

t_sjson_error						new_value(t_sjson_env *e,
											t_sjson_value *out)
{
	int					error;

	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
		return (out->error = error);
	if (evaluate_type(e, out) == SJSON_TYPE_INVALID)
		error = sjson_error(e, SJSON_ERROR_INVALID_SYNTAX, "new_value");
	if (out->type == SJSON_TYPE_BOOLEAN || out->type == SJSON_TYPE_NULL)
		error = SJSON_ERROR_OK;
	if (out->type == SJSON_TYPE_STRING)
		error = string_extractor(e, &out->data.str);
	if (out->type == SJSON_TYPE_REAL)
		error = new_real(e, out);
	if (out->type == SJSON_TYPE_OBJECT)
		error = new_object(e, out);
	if (out->type == SJSON_TYPE_ARRAY)
		error = new_array(e, out);
	out->error = error;
	return (error);
}
