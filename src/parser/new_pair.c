/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_pair.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 20:11:59 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:59:31 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"
#include <string.h> //FIXME

t_sjson_error	pair_extractor(t_sjson_env *e,
								t_sjson_pair *out,
								t_sjson_value *parent)
{
	t_sjson_error	error;

	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
		return (error);
	if ((error = string_extractor(e, &out->key)) != SJSON_ERROR_OK)
		return (error);
	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
		return (error);
	if (strchr(SJSON_PAIR_SEPARATORS, e->src[e->pos]) == NULL)
		return (sjson_error(e, SJSON_ERROR_INVALID_SEPARATOR,
			"pair_extractor"));
		++e->pos;
	if ((error = jump_blanks(e)) != SJSON_ERROR_OK)
		return (error);
	if ((out->value = malloc(sizeof(t_sjson_value))) == NULL)
	{
		free(out->key);
		return (sjson_error(e, SJSON_ERROR_OUT_OF_MEMORY, "pair_extractor"));
	}
	out->value->parent = parent;
	return (new_value(e, out->value));
}
