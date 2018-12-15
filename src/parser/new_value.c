/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/29 17:02:02 by hmartzol          #+#    #+#             */
/*   Updated: 2018/12/15 18:10:27 by null             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <sjson_defines.h>
//#include <sjson_types.h>

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"
#include "../../inc/sjson_std_functions.h"

#include <stdio.h> //FIXME

static inline t_sjson_value_type	evaluate_type(t_sjson_env *e,
												t_sjson_value *out)
{
	if (in_set(e, SJSON_OBJECT_STARTERS, SJSON_OBJECT_COUNT, 1)
			== SJSON_ERROR_OK)
		return (out->type = object);
	if (in_set(e, SJSON_ARRAY_STARTERS, SJSON_ARRAY_COUNT, 1) == SJSON_ERROR_OK)
		return (out->type = array);
	if (in_set(e, SJSON_NULL_SET, SJSON_NULL_COUNT, 1) == SJSON_ERROR_OK)
		return (out->type = null);
	if (in_set(e, SJSON_TRUE_SET, SJSON_TRUE_COUNT, 1) == SJSON_ERROR_OK)
	{
		out->data.bol = 1;
		return (out->type = boolean);
	}
	if (in_set(e, SJSON_FALSE_SET, SJSON_FALSE_COUNT, 1) == SJSON_ERROR_OK)
	{
		out->data.bol = 0;
		return (out->type = boolean);
	}
	if (e->src[e->pos] == '-' || (SJSON_EXTEND && e->src[e->pos] == '+')
			|| (std_isdigit(e->src[e->pos])
				&& (SJSON_EXTEND || e->src[e->pos] != '0'))
			|| (SJSON_EXTEND && e->src[e->pos] == '.'))
		return (out->type = real);
	return (out->type = invalid);
}

static inline t_sjson_error			treat_errors(t_sjson_env *e,
												t_sjson_value *out,
												t_sjson_error error)
{
	int				line;
	int				column;
	int				len;
	unsigned char	*start;
	const char *error_strings[] = {
		"ok", "end of file reached", "out of memory", "invalid syntax",
		"invalid separator", "invalid bound", "invalid ending bound",
		"invalid parameter", "mismatched node type", "target not found",
		"missing comment ender"};

	if (error != SJSON_ERROR_OK)
	{
		line = 0;
		column = 0;
		len = e->pos;
		start = e->src;
		while (len--)
		{
			if (!line)
				++column;
			if (e->src[len] == '\n')
				if (!line++)
					start = &e->src[len];
		}
		len = 0;
		while (&start[len] < e->limit
				&& start[len] != '\n' && start[len] != '\0')
			++len;
		printf("JSON parsing error: %s\n", error_strings[error]);
		column += printf("L:%d|C:%d|", line, column);
		printf("%.*s\n", len, start);
		len = -1;
		while (++len < column)
			printf(" ");
		printf("^\n");
	}
	out->error = error;
	return (error);
}

t_sjson_error						new_value(t_sjson_env *e,
											t_sjson_value *out)
{
	int					error;

	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
		return (out->error = error);
	if (evaluate_type(e, out) == invalid)
		error = SJSON_INVALID_SYNTAX;
	if (out->type == boolean || out->type == null)
		error = SJSON_ERROR_OK;
	if (out->type == string)
		error = string_extractor(e, &out->data.str);
	if (out->type == real)
		error = new_real(e, out);
	if (out->type == object)
		error = new_object(e, out);
	if (out->type == array)
		error = new_array(e, out);
	return (treat_errors(e, out, error));
}
