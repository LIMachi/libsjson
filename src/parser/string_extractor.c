/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_extractor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 10:26:28 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/12 17:08:46 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"

/*
** will try to extract the bound englobed string starting at src
** and will no go to or past the limit ([src, limit[)
*/

t_sjson_error	string_extractor(t_sjson_env *e,
								t_sjson_string **out)
{
	char	*bound;
	size_t	n;
	size_t	len;

	if (!(bound = sstrchr(SJSON_STRING_BOUNDS, e->src[e->pos])))
		return (sjson_error(e, SJSON_ERROR_INVALID_BOUND, "string_extractor"));
	len = 1;
	while (len + e->pos < e->slimit && e->src[len + e->pos] != '\0'
			&& !(e->src[len + e->pos] == *bound
				&& e->src[len - 1 + e->pos] != '\\'))
		++len;
	if (len == 1 || len + e->pos == e->slimit || e->src[len + e->pos] == '\0')
		return (sjson_error(e, SJSON_ERROR_MISSING_ENDING_BOUND,
			"string_extractor"));
		--len;
	if ((*out = malloc(sizeof(t_sjson_string) + len + 1)) == NULL)
		return (sjson_error(e, SJSON_ERROR_OUT_OF_MEMORY, "string_extractor"));
	(*out)->length = len;
	(*out)->data[len] = '\0';
	n = len;
	while (n--)
		(*out)->data[n] = e->src[e->pos + 1 + n];
	e->pos += 2 + len;
	return (SJSON_ERROR_OK);
}
