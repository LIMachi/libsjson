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
#include <libft.h>

static inline t_sjson_error	new_object_1(t_sjson_env *e,
										t_sjson_value *out,
										size_t *tmp_size)
{
	t_sjson_pair	**tmp_ptr;
	t_sjson_error	error;

	if (out->data.obj.nb_pairs >= *tmp_size)
	{
		if ((tmp_ptr = realloc(out->data.obj.pairs,
				(*tmp_size <<= 1) * sizeof(t_sjson_pair*))) == NULL)
			return (SJSON_ERROR_OUT_OF_MEMORY);
		else
			out->data.obj.pairs = tmp_ptr;
	}
	if ((out->data.obj.pairs[out->data.obj.nb_pairs] =
		malloc(sizeof(t_sjson_pair))) == NULL)
		return (SJSON_ERROR_OUT_OF_MEMORY);
	if ((error = pair_extractor(e, out->data.obj.pairs[out->data.obj.nb_pairs],
			out)) != SJSON_ERROR_OK)
		return (error);
	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
		return (error);
	if (ft_strchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) == NULL &&
	in_set(e, SJSON_OBJECT_ENDERS, SJSON_OBJECT_COUNT, 0) != SJSON_ERROR_OK)
		return (SJSON_ERROR_INVALID_SYNTAX);
	++out->data.obj.nb_pairs;
	return (SJSON_ERROR_OK);
}

t_sjson_error				new_object(t_sjson_env *e,
									t_sjson_value *out)
{
	size_t			tmp_size;
	t_sjson_pair	**tmp_ptr;
	t_sjson_error	error;

	tmp_size = SJSON_STARTING_OBJECT_SIZE;
	out->data.obj.nb_pairs = 0;
	if ((out->data.obj.pairs =
		malloc(sizeof(t_sjson_pair*) * tmp_size)) == NULL)
		return (SJSON_ERROR_OUT_OF_MEMORY);
	while ((error = jump_blanks(e)) == SJSON_ERROR_OK && in_set(e,
			SJSON_OBJECT_ENDERS, SJSON_OBJECT_COUNT, 1) != SJSON_ERROR_OK)
		if ((error = new_object_1(e, out, &tmp_size)) != SJSON_ERROR_OK)
			return (error);
	if (tmp_size > out->data.obj.nb_pairs)
	{
		if ((tmp_ptr = realloc(out->data.obj.pairs,
				out->data.obj.nb_pairs * sizeof(t_sjson_pair*))) == NULL)
			return (SJSON_ERROR_OUT_OF_MEMORY);
		else
			out->data.obj.pairs = tmp_ptr;
	}
	return (error);
}

/*
** t_sjson_error	new_object(t_sjson_env *e,
** 						t_sjson_value *out)
** {
** 	size_t			tmp_size;
** 	t_sjson_pair	**tmp_ptr;
** 	t_sjson_error	error;
** 	tmp_size = SJSON_STARTING_OBJECT_SIZE;
** 	out->data.obj.nb_pairs = 0;
** 	if ((out->data.obj.pairs
** 			= malloc(sizeof(t_sjson_pair*) * tmp_size)) == NULL)
** 		return (SJSON_ERROR_OUT_OF_MEMORY);
** 	while ((error = jump_blanks(e)) == SJSON_ERROR_OK && in_set(e,
** 		SJSON_OBJECT_ENDERS, SJSON_OBJECT_COUNT, 1) != SJSON_ERROR_OK)
** 	{
** 		if (out->data.obj.nb_pairs >= tmp_size)
** 		{
** 			if ((tmp_ptr = realloc(out->data.obj.pairs,
** 					(tmp_size <<= 1) * sizeof(t_sjson_pair*))) == NULL)
** 				return (SJSON_ERROR_OUT_OF_MEMORY);
** 			else
** 				out->data.obj.pairs = tmp_ptr;
** 		}
** 		if ((out->data.obj.pairs[out->data.obj.nb_pairs]
** 				= malloc(sizeof(t_sjson_pair))) == NULL)
** 			return (SJSON_ERROR_OUT_OF_MEMORY);
** 		pair_extractor(e, out->data.obj.pairs[out->data.obj.nb_pairs], parent);
** 		++out->data.obj.nb_pairs;
** 		if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
** 			return (error);
** 		if (ft_strchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) == NULL &&
** 		in_set(e, SJSON_OBJECT_ENDERS, SJSON_OBJECT_COUNT, 0) != SJSON_ERROR_OK)
** 			return (SJSON_ERROR_INVALID_SYNTAX);
** 	}
** 	if (tmp_size > out->data.obj.nb_pairs)
** 	{
** 		if ((tmp_ptr = realloc(out->data.obj.pairs,
** 				out->data.obj.nb_pairs * sizeof(t_sjson_pair*))) == NULL)
** 			return (SJSON_ERROR_OUT_OF_MEMORY);
** 		else
** 			out->data.obj.pairs = tmp_ptr;
** 	}
** 	return (error);
** }
*/
