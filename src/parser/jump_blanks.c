/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jump_blanks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 11:36:40 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 20:13:47 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <sjson_defines.h>

# include "../../inc/sjson_defines.h"
# include "../../inc/sjson_types.h"
# include "../../inc/sjson_std_functions.h"

#if SJSON_EXTEND

/*
** jump all characters defined in SJSON_BLANKS starting from e->pos in e->src
** will not go further than e->slimit
** if SJSON_EXTENDED is TRUE, then will also jump comments
*/

t_sjson_error	jump_blanks(t_sjson_env *e)
{
	size_t	comment_type;

	while (42)
	{
		while (e->pos < e->slimit && e->src[e->pos] != '\0'
				&& std_strchr(SJSON_BLANKS, e->src[e->pos]) != NULL)
			++e->pos;
		if (e->pos == e->slimit || e->src[e->pos] == '\0')
			return (SJSON_ERROR_EOF);
		comment_type = 0;
		while (comment_type < SJSON_COMMENT_COUNT
				&& std_strncmp(SJSON_COMMENT_STARTERS[comment_type],
					&e->src[e->pos], e->slimit - pos))
			++comment_type;
		if (comment_type == SJSON_COMMENT_COUNT)
			return (SJSON_ERROR_OK);
		while (e->pos < e->slimit && e->src[e->pos] != '\0'
				&& std_strncmp(SJSON_COMMENT_ENDERS[comment_type], &e->src[e->pos],
					e->slimit - pos))
			++e->pos;
		if (e->pos == e->slimit || e->src[e->pos] == '\0')
			return (SJSON_ERROR_MISSING_COMMENT_ENDER);
	}
	return (SJSON_ERROR_THE_FUCK_IS_THAT);
}

#else

t_sjson_error	jump_blanks(t_sjson_env *e)
{
	while (e->pos < e->slimit && e->src[e->pos] != '\0'
			&& std_strchr(SJSON_BLANKS, e->src[e->pos]) != NULL)
		++e->pos;
	if (e->pos == e->slimit && e->src[e->pos] == '\0')
		return (SJSON_ERROR_EOF);
	return (SJSON_ERROR_OK);
}

#endif
