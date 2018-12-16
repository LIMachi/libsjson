/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 20:38:47 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/12 16:42:46 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"
#include <libft.h>

static inline int	i_sjson_extract_line(t_sjson_env *e,
										int *line,
										int *column,
										char **start)
{
	int len;

	*line = 0;
	*column = 0;
	len = e->pos;
	*start = e->src;
	while (len--)
	{
		if (!line)
			++column;
		if (e->src[len] == '\n')
			if (!line++)
				*start = &e->src[len];
	}
	len = 0;
	while (&(*start)[len] < e->limit && (*start)[len] != '\n'
			&& (*start)[len] != '\0')
		++len;
	return (len);
}

t_sjson_error		sjson_error(t_sjson_env *e, t_sjson_error err, char *func)
{
	int			len;
	int			column;
	int			line;
	char		*start;
	const char	*error_strings[] = {"ok", "end of file reached",
		"out of memory", "invalid syntax", "invalid separator", "invalid bound",
		"invalid ending bound", "invalid parameter", "mismatched node type",
		"target not found", "missing comment ender"};

	if (e->flags & SJSON_FLAG_PRINT_ERRORS && err != SJSON_ERROR_OK)
	{
		ft_dprintf(e->fd_error, "%s: JSON parsing error: %s\n", func,
		(err < 11 && err > 0) ? error_strings[err] : "how did you get this?");
		len = i_sjson_extract_line(e, &line, &column, &start);
		column += ft_dprintf(e->fd_error, "%d:%d:", line + 1, column + 1);
		line = start[len];
		start[len] = '\0';
		ft_dprintf(e->fd_error, "%s\n", start);
		start[len] = line;
		len = -1;
		while (++len < column)
			write(e->fd_error, " ", 1);
		write(e->fd_error, "^\n", 2);
	}
	return (err);
}
