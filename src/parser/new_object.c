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
#include <string.h> //FIXME

/*
** #ifndef __FreeBSD__
** static void	*reallocf(void *ptr, size_t size)
** {
** 	void	*out;
** 	if ((out = realloc(ptr, size)) == NULL && ptr != NULL && size != 0)
** 		free(ptr);
** 	return (out);
** }
** #endif
*/

static inline t_sjson_error	new_object_1(t_sjson_env *e,
										t_sjson_value *out)
{
	t_sjson_error	error;
	t_sjson_pair	pair;

	if ((error = pair_extractor(e, &pair, out)) != SJSON_ERROR_OK)
		return (error);
	if (ft_swiss_table_insert(&out->data.obj, strdup(pair.key->data), pair.value))
		return (SJSON_ERROR_OUT_OF_MEMORY);
	free(pair.key);
	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
		return (error);
	if (strchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) == NULL &&
	in_set(e, SJSON_OBJECT_ENDERS, SJSON_OBJECT_COUNT, 0) != SJSON_ERROR_OK)
		return (SJSON_ERROR_INVALID_SYNTAX);
	if (strchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) != NULL)
		++e->pos;
	return (SJSON_ERROR_OK);
}

/*
** static inline t_sjson_error	new_object_1(t_sjson_env *e,
** 										t_sjson_value *out,
** 										size_t *tmp_size)
** {
** 	t_sjson_pair	**tmp_ptr;
** 	t_sjson_error	error;
** 	if (out->data.obj.nb_pairs >= *tmp_size)
** 	{
** 		if ((tmp_ptr = realloc(out->data.obj.pairs,
** 				(*tmp_size <<= 1) * sizeof(t_sjson_pair*))) == NULL)
** 			return (SJSON_ERROR_OUT_OF_MEMORY);
** 		else
** 			out->data.obj.pairs = tmp_ptr;
** 	}
** 	if ((out->data.obj.pairs[out->data.obj.nb_pairs] =
** 		malloc(sizeof(t_sjson_pair))) == NULL)
** 		return (SJSON_ERROR_OUT_OF_MEMORY);
** 	if ((error = pair_extractor(e, out->data.obj.pairs[out->data.obj.nb_pairs],
** 			out)) != SJSON_ERROR_OK)
** 		return (error);
** 	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
** 		return (error);
** 	if (strchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) == NULL &&
** 	in_set(e, SJSON_OBJECT_ENDERS, SJSON_OBJECT_COUNT, 0) != SJSON_ERROR_OK)
** 		return (SJSON_ERROR_INVALID_SYNTAX);
** 	++out->data.obj.nb_pairs;
** 	if (strchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) != NULL)
** 		++e->pos;
** 	return (SJSON_ERROR_OK);
** }
*/

/*
** t_sjson_error				new_object(t_sjson_env *e,
** 									t_sjson_value *out)
** {
** 	size_t			tmp_size;
** 	t_sjson_error	error;
** 	tmp_size = SJSON_STARTING_OBJECT_SIZE;
** 	out->data.obj.nb_pairs = 0;
** 	if ((out->data.obj.pairs =
** 		malloc(sizeof(t_sjson_pair*) * tmp_size)) == NULL)
** 		return (SJSON_ERROR_OUT_OF_MEMORY);
** 	while ((error = jump_blanks(e)) == SJSON_ERROR_OK && in_set(e,
** 			SJSON_OBJECT_ENDERS, SJSON_OBJECT_COUNT, 1) != SJSON_ERROR_OK)
** 		if ((error = new_object_1(e, out, &tmp_size)) != SJSON_ERROR_OK)
** 			return (error);
** 	if (tmp_size > out->data.obj.nb_pairs)
** 		out->data.obj.pairs = reallocf(out->data.obj.pairs,
** 				out->data.obj.nb_pairs * sizeof(t_sjson_pair*));
** 	return (error);
** }
*/

t_sjson_error				new_object(t_sjson_env *e,
									t_sjson_value *out)
{
	t_sjson_error	error;

	out->data.obj = ft_swiss_table_create((t_swt_hashfun)ft_basic_hash,
		(t_swt_cmpfun)strcmp);
	if (out->data.obj.nb_groups == 0)
		return (SJSON_ERROR_OUT_OF_MEMORY);
	while ((error = jump_blanks(e)) == SJSON_ERROR_OK && in_set(e,
			SJSON_OBJECT_ENDERS, SJSON_OBJECT_COUNT, 1) != SJSON_ERROR_OK)
		if ((error = new_object_1(e, out)) != SJSON_ERROR_OK)
			return (error);
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
