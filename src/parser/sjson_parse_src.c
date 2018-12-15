/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_parse_src.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 19:08:18 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:04:26 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/sjson_defines.h"
# include "../../inc/sjson_types.h"
# include "../../inc/sjson_functions.h"

t_sjson_error	sjson_parse_src(unsigned char *src,
								size_t len,
								t_sjson_value **out)
{
	t_sjson_env		e;

	if (src == NULL || len == 0 || out == NULL)
		return (SJSON_INVALID_PARAMETER);
	if ((*out = malloc(sizeof(t_sjson_value))) == NULL)
		return (SJSON_ERROR_OOM);
	e.src = src;
	e.limit = src + len;
	e.slimit = len;
	e.pos = 0;
	(*out)->parent = *out;
	return (new_value(&e, *out));
}