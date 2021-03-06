/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 20:38:47 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/12 16:42:46 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sjson.h>
#include <sjson_functions.h>
#include <unistd.h>
#include <stdio.h>

static inline int		sjson_print_2(int fd,
									t_sjson_value *val,
									int flags,
									int out)
{
	size_t	i;

	if (val->type == SJSON_TYPE_ARRAY)
	{
		out += write(fd, SJSON_ARRAY_STARTERS[0],
			sstrlen(SJSON_ARRAY_STARTERS[0]));
		i = -1;
		while (++i < val->data.ar.nb_values)
		{
			out += sjson_print(fd, val->data.ar.values[i], flags);
			if (i + 1 < val->data.ar.nb_values)
				out += write(fd, SJSON_ARRAY_SEPARATORS, 1);
		}
		out += write(fd, SJSON_ARRAY_ENDERS[0],
			sstrlen(SJSON_ARRAY_ENDERS[0]));
	}
	return (out);
}

static inline int		sjson_print_1(int fd,
									t_sjson_value *val,
									int flags,
									int out)
{
	size_t			i;
	size_t			c;
	char			*key;
	t_sjson_value	*value;

	out += sjson_print_2(fd, val, flags, out);
	if (val->type == SJSON_TYPE_OBJECT)
	{
		out += write(fd, SJSON_OBJECT_STARTERS[0],
			sstrlen(SJSON_OBJECT_STARTERS[0]));
		i = 0;
		c = 0;
		while (ft_swiss_table_iterate(&val->data.obj, &i, (void**)&key,
			(void**)&value))
		{
			out += write(fd, SJSON_STRING_BOUNDS, 1) + write(fd, key,
				sstrlen(key)) + write(fd, SJSON_STRING_BOUNDS, 1) + write(fd,
					SJSON_PAIR_SEPARATORS, 1) + sjson_print(fd, value, flags);
			if (++c < val->data.obj.pair_count)
				out += write(fd, SJSON_ARRAY_SEPARATORS, 1);
		}
		out += write(fd, SJSON_OBJECT_ENDERS[0],
			sstrlen(SJSON_OBJECT_ENDERS[0]));
	}
	return (out);
}

int						sjson_print(int fd,
									t_sjson_value *val,
									int flags)
{
	int	out;

	out = 0;
	if (val->error != SJSON_ERROR_OK && (flags & SJSON_FLAG_PRINT_ERRORS))
	{
		out += write(fd, "\\\\ERROR:", 8);
		val->error += '0';
		out += write(fd, &val->error, 1);
		val->error -= '0';
		out += write(fd, "\\\\", 2);
	}
	if (val->type == SJSON_TYPE_NULL)
		out += write(fd, "null", 4);
	if (val->type == SJSON_TYPE_BOOLEAN)
		out += write(fd, val->data.bol ? "true" : "false", 4 + !val->data.bol);
	if (val->type == SJSON_TYPE_REAL)
		out += dprintf(fd, "%f", (double)val->data.real);
	if (val->type == SJSON_TYPE_STRING)
	{
		out += write(fd, SJSON_STRING_BOUNDS, 1);
		out += write(fd, val->data.str->data, val->data.str->length);
		out += write(fd, SJSON_STRING_BOUNDS, 1);
	}
	return (sjson_print_1(fd, val, flags, out));
}
