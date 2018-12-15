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

// #include <sjson_defines.h>
// #include <sjson_types.h>

# include "../../inc/sjson_defines.h"
# include "../../inc/sjson_types.h"
# include "../../inc/sjson_functions.h"
# include "../../inc/sjson_std_functions.h"

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
			return (SJSON_ERROR_OOM);
		else
			out->data.ar.values = tmp_ptr;
	}
	if ((out->data.ar.values[out->data.ar.nb_values]
			= malloc(sizeof(t_sjson_value))) == NULL)
		return (SJSON_ERROR_OOM);
	out->data.ar.values[out->data.ar.nb_values]->parent = out;
	if ((error = new_value(e,
			out->data.ar.values[out->data.ar.nb_values])) != SJSON_ERROR_OK)
		return (error);
	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
		return (error);
	if (std_strchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) == NULL &&
	in_set(e, SJSON_ARRAY_ENDERS, SJSON_ARRAY_COUNT, 0) != SJSON_ERROR_OK)
		return (SJSON_INVALID_SYNTAX);
	++out->data.ar.nb_values;
	return (SJSON_ERROR_OK);
}

t_sjson_error				new_array(t_sjson_env *e,
									t_sjson_value *out)
{
	size_t			tmp_size;
	t_sjson_value	**tmp_ptr;
	t_sjson_error	error;

	tmp_size = SJSON_STARTING_ARRAY_SIZE;
	out->data.ar.nb_values = 0;
	if ((out->data.ar.values
			= malloc(sizeof(t_sjson_value*) * tmp_size)) == NULL)
		return (SJSON_ERROR_OOM);
	while ((error = jump_blanks(e)) == SJSON_ERROR_OK && in_set(e,
			SJSON_ARRAY_ENDERS, SJSON_ARRAY_COUNT, 1) != SJSON_ERROR_OK)
		if ((error = new_array_1(e, out, &tmp_size)) != SJSON_ERROR_OK)
			return (error);
	if (tmp_size > out->data.ar.nb_values)
	{
		if ((tmp_ptr = realloc(out->data.ar.values,
				out->data.ar.nb_values * sizeof(t_sjson_value*))) == NULL)
			return (SJSON_ERROR_OOM);
		else
			out->data.ar.values = tmp_ptr;
	}
	return (error);
}

/*
** t_sjson_error	new_array(t_sjson_env *e,
** 							t_sjson_value *out)
** {
** 	size_t			tmp_size;
** 	t_sjson_value	**tmp_ptr;
** 	t_sjson_error	error;
** 
** 	tmp_size = SJSON_STARTING_ARRAY_SIZE;
** 	out->data.ar.nb_values = 0;
** 	if ((out->data.ar.values
** 			= malloc(sizeof(t_sjson_value*) * tmp_size)) == NULL)
** 		return (SJSON_ERROR_OOM);
** 	while ((error = jump_blanks(e)) == SJSON_ERROR_OK && in_set(e,
** 		SJSON_ARRAY_ENDERS, SJSON_ARRAY_COUNT, 1) != SJSON_ERROR_OK)
** 	{
** 		if (out->data.ar.nb_values >= tmp_size)
** 		{
** 			if ((tmp_ptr = realloc(out->data.ar.values,
** 					(tmp_size <<= 1) * sizeof(t_sjson_value*))) == NULL)
** 				return (SJSON_ERROR_OOM);
** 			else
** 				out->data.ar.values = tmp_ptr;
** 		}
** 		if ((out->data.ar.values[out->data.ar.nb_values]
** 				= malloc(sizeof(t_sjson_value))) == NULL)
** 			return (SJSON_ERROR_OOM);
** 		out->data.ar.values[out->data.ar.nb_values]->parent = out;
** 		new_value(e, out->data.ar.values[out->data.ar.nb_values]);
** 		++out->data.ar.nb_values;
** 		if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
** 			return (error);
** 		if (std_strchr(SJSON_ARRAY_SEPARATORS, e->src[e->pos]) == NULL &&
** 		in_set(e, SJSON_ARRAY_ENDERS, SJSON_ARRAY_COUNT, 0) != SJSON_ERROR_OK)
** 			return (SJSON_INVALID_SYNTAX);
** 	}
** 	if (tmp_size > out->data.ar.nb_values)
** 	{
** 		if ((tmp_ptr = realloc(out->data.ar.values,
** 				out->data.ar.nb_values * sizeof(t_sjson_value*))) == NULL)
** 			return (SJSON_ERROR_OOM);
** 		else
** 			out->data.ar.values = tmp_ptr;
** 	}
** 	return (error);
** }
*/