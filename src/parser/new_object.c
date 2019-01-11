/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 16:57:21 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/12 17:21:27 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"

static inline t_sjson_error	new_object_1(t_sjson_env *e,
										t_sjson_value *out)
{
	t_sjson_error	error;
	t_sjson_pair	pair;

	if ((error = pair_extractor(e, &pair, out)) != SJSON_ERROR_OK)
		return (error);
	if (ft_swiss_table_insert(&out->data.obj,
			sstrdup(pair.key->data), pair.value))
		return (SJSON_ERROR_OUT_OF_MEMORY);
	free(pair.key);
	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
		return (error);
	if (sstrchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) == NULL &&
	in_set(e, SJSON_OBJECT_ENDERS, SJSON_OBJECT_COUNT, 0) != SJSON_ERROR_OK)
		return (SJSON_ERROR_INVALID_SYNTAX);
	if (sstrchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) != NULL)
		++e->pos;
	return (SJSON_ERROR_OK);
}

t_sjson_error				new_object(t_sjson_env *e,
									t_sjson_value *out)
{
	t_sjson_error	error;

	out->data.obj = ft_swiss_table_create((t_swt_hashfun)ft_basic_hash,
		(t_swt_cmpfun)sstrcmp);
	if (out->data.obj.nb_groups == 0)
		return (SJSON_ERROR_OUT_OF_MEMORY);
	while ((error = jump_blanks(e)) == SJSON_ERROR_OK && in_set(e,
			SJSON_OBJECT_ENDERS, SJSON_OBJECT_COUNT, 1) != SJSON_ERROR_OK)
		if ((error = new_object_1(e, out)) != SJSON_ERROR_OK)
			return (error);
	return (error);
}
