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

// #include <sjson_defines.h>
// #include <sjson_types.h>

# include "../../inc/sjson_defines.h"
# include "../../inc/sjson_types.h"
# include "../../inc/sjson_std_functions.h"

/*
** will try to extract the SJSON_STRING_BOUNDS englobed string starting at src
** and will no go to or past the limit ([src, limit[)
*/

t_sjson_error	string_extractor(t_sjson_env *e,
								t_sjson_string **out)
{
	char	*bound;
	size_t	len;

	if (!(bound = std_strchr(SJSON_STRING_BOUNDS, e->src[e->pos])))
		return (SJSON_INVALID_BOUND);
	len = 1;
	while (len + e->pos < e->slimit && e->src[len + e->pos] != '\0'
			&& !(e->src[len + e->pos] == *bound
				&& e->src[len - 1 + e->pos] != '\\'))
		++len;
	if (len == 1 || len + e->pos == e->slimit || e->src[len + e->pos] == '\0')
		return (SJSON_MISSING_ENDING_BOUND);
	--len;
	if ((*out = malloc(sizeof(t_sjson_string) + len)) == NULL)
		return (SJSON_ERROR_OOM);
	(*out)->length = len;
	(void)std_memcpy((*out)->data, &e->src[e->pos + 1], len);
	e->pos += 2 + len;
	return (SJSON_ERROR_OK);
}
