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

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"

#if SJSON_EXTEND

/*
** jump all characters defined in SJSON_BLANKS starting from e->pos in e->src
** will not go further than e->slimit
** if SJSON_EXTENDED is TRUE, then will also jump comments
*/

t_sjson_error	jump_blanks(t_sjson_env *e)
{
	size_t				comment_type;
	static const char	*starters[3] = {"#", "//", "/*"};
	static const char	*enders[3] = {"\n", "\n", "*/"};

	while (42)
	{
		while (e->pos < e->slimit && e->src[e->pos] != '\0'
				&& sstrchr(SJSON_BLANKS, e->src[e->pos]) != NULL)
			++e->pos;
		if (e->pos == e->slimit || e->src[e->pos] == '\0')
			return (SJSON_ERROR_END_OF_FILE);
		comment_type = 0;
		while (comment_type < 3 && sstrncmp((char*)starters[comment_type],
					&e->src[e->pos], e->slimit - e->pos))
			++comment_type;
		if (comment_type == 3)
			return (SJSON_ERROR_OK);
		while (e->pos < e->slimit && e->src[e->pos] != '\0'
				&& sstrncmp((char*)enders[comment_type], &e->src[e->pos],
					e->slimit - e->pos))
			++e->pos;
		if (e->pos == e->slimit || e->src[e->pos] == '\0')
			return (SJSON_ERROR_MISSING_COMMENT_ENDER);
	}
	return (SJSON_ERROR_KO);
}

#else

t_sjson_error	jump_blanks(t_sjson_env *e)
{
	while (e->pos < e->slimit && e->src[e->pos] != '\0'
			&& sstrchr(SJSON_BLANKS, e->src[e->pos]) != NULL)
		++e->pos;
	if (e->pos == e->slimit && e->src[e->pos] == '\0')
		return (sjson_error(e, SJSON_ERROR_END_OF_FILE, "jump_blanks"));
	return (SJSON_ERROR_OK);
}

#endif
