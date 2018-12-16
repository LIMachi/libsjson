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

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"

t_sjson_error	sjson_parse_src(char *src,
								t_sjson_value **out,
								t_sjson_flags flags,
								int fd_error)
{
	t_sjson_env		e;

	if (src == NULL || out == NULL)
		return (SJSON_ERROR_INVALID_PARAMETER);
	if ((*out = malloc(sizeof(t_sjson_value))) == NULL)
		return (SJSON_ERROR_OUT_OF_MEMORY);
	e.src = src;
	e.limit = (char*)(size_t)-1;
	e.slimit = (size_t)-1;
	e.pos = 0;
	e.fd_error = fd_error;
	e.flags = flags;
	(*out)->parent = *out;
	return (new_value(&e, *out));
}
