/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 20:07:24 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/12 17:23:28 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"

#if !defined(__FreeBSD__) && !defined(__APPLE__)

static void					*reallocf(void *ptr, size_t size)
{
	void	*out;

	if ((out = realloc(ptr, size)) == NULL && ptr != NULL && size != 0)
		free(ptr);
	return (out);
}

#endif

static inline t_sjson_error	new_array_1(t_sjson_env *e,
										t_sjson_value *out,
										size_t *tmp_size)
{
	t_sjson_value	**tmp_ptr;
	t_sjson_error	error;

	if (out->data.ar.nb_values >= *tmp_size)
	{
		if ((tmp_ptr = realloc(out->data.ar.values,
				(*tmp_size <<= 1) * sizeof(t_sjson_value*))) == NULL)
			return (sjson_error(e, SJSON_ERROR_OUT_OF_MEMORY, "new_array_1"));
		else
			out->data.ar.values = tmp_ptr;
	}
	if ((out->data.ar.values[out->data.ar.nb_values] =
			malloc(sizeof(t_sjson_value))) == NULL)
		return (sjson_error(e, SJSON_ERROR_OUT_OF_MEMORY, "new_array_1"));
	out->data.ar.values[out->data.ar.nb_values]->parent = out;
	if ((error = new_value(e,
			out->data.ar.values[out->data.ar.nb_values])) != SJSON_ERROR_OK)
		return (error);
	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
		return (error);
	if (sstrchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) == NULL &&
	in_set(e, SJSON_ARRAY_ENDERS, SJSON_ARRAY_COUNT, 0) != SJSON_ERROR_OK)
		return (sjson_error(e, SJSON_ERROR_INVALID_SYNTAX, "new_array_1"));
	++out->data.ar.nb_values;
	return (SJSON_ERROR_OK);
}

t_sjson_error				new_array(t_sjson_env *e,
									t_sjson_value *out)
{
	size_t			tmp_size;
	t_sjson_error	error;

	tmp_size = SJSON_STARTING_ARRAY_SIZE;
	out->data.ar.nb_values = 0;
	if ((out->data.ar.values =
			malloc(sizeof(t_sjson_value*) * tmp_size)) == NULL)
		return (sjson_error(e, SJSON_ERROR_OUT_OF_MEMORY, "new_array 1"));
	while ((error = jump_blanks(e)) == SJSON_ERROR_OK && in_set(e,
			SJSON_ARRAY_ENDERS, SJSON_ARRAY_COUNT, 1) != SJSON_ERROR_OK)
		if ((error = new_array_1(e, out, &tmp_size)) != SJSON_ERROR_OK)
			return (error);
		else if (sstrchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) != NULL)
			++e->pos;
	if (tmp_size > out->data.ar.nb_values)
		out->data.ar.values = reallocf(out->data.ar.values,
			out->data.ar.nb_values * sizeof(t_sjson_value*));
	return (error);
}
