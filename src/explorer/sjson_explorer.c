/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_explorer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 19:32:04 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 20:08:21 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"
#include <stdarg.h>

#include <libft.h>

inline static int	i_right(t_jae *e, va_list ap)
{
	t_sjson_value	*tmp;
	int				tmp1;
	char			*tmp2;

	if (e->error_stack)
		++e->error_stack;
	else if ((e->etype != SJSON_TYPE_OBJECT && e->etype != SJSON_TYPE_ARRAY) ||
			!sjson_test_type(e->node, e->etype))
		e->error_stack = 1;
	if (e->error_stack)
		return (va_arg(ap, size_t) || 1);
	if (e->etype == SJSON_TYPE_OBJECT)
	{
		if (sjson_search_pair_in_object(e->node,
				tmp2 = va_arg(ap, char *), &tmp) != SJSON_ERROR_OK)
			return (e->error_stack = 1);
		e->node = tmp;
		return (1);
	}
	if (sjson_search_index_in_array(e->node,
			tmp1 = va_arg(ap, size_t), &tmp) != SJSON_ERROR_OK)
		return (e->error_stack = 1);
	e->node = tmp;
	return (1);
}

inline static int	i_store(t_jae *e, void *tmp, int do_store)
{
	if (do_store)
	{
		if (e->node->type == SJSON_TYPE_STRING)
			*(t_sjson_string **)tmp = e->node->data.str;
		if (e->node->type & SJSON_TYPE_NULL)
			*(void **)tmp = NULL;
		if (e->node->type & SJSON_TYPE_ARRAY)
			*(t_sjson_array **)tmp = &e->node->data.ar;
		if (e->node->type & SJSON_TYPE_OBJECT)
			*(t_sjson_object **)tmp = &e->node->data.obj;
		if (e->node->type == SJSON_TYPE_REAL)
			*(t_sjson_real *)tmp = e->node->data.real;
		if (e->node->type == SJSON_TYPE_BOOLEAN)
			*(t_sjson_boolean *)tmp = e->node->data.bol;
		if (e->node->type == SJSON_TYPE_NONE)
			*(t_sjson_value **)tmp = e->node;
	}
	++e->valid;
	return (1);
}

inline static int	i_access(t_jae *e, const char c, va_list ap)
{
	t_sjson_call_back	cb;

	if (c == '>')
		return (i_right(e, ap));
	if (c == '<' && e->error_stack && --e->error_stack)
		return (1);
	if (c == '<')
		return ((e->node = e->node->parent) == e->node);
	if (c == '#' || c == '*')
		cb = va_arg(ap, t_sjson_call_back);
	if (e->error_stack || !sjson_test_type(e->node, e->etype))
		return ((c == '#' ? va_arg(ap, void*) : 0) || 1);
	if (c == '#')
	{
		if (e->etype == SJSON_TYPE_NONE)
			e->e = cb(e->node, va_arg(ap, void*), e->node->type);
		else if (e->etype == SJSON_TYPE_STRING)
			e->e = cb(e->node->data.str, va_arg(ap, void*), e->node->type);
		else
			e->e = cb((void *)&e->node->data, va_arg(ap, void*), e->node->type);
		return (e->e != SJSON_ERROR_OK || (++e->valid) || 1);
	}
	else if (c == '*' || c == '?')
		return (i_store(e, (void*)cb, c == '*'));
	return (0);
}

inline static int	i_type_change(t_jae *e, const char c)
{
	if (c == 'o')
		return ((e->etype = SJSON_TYPE_OBJECT) == SJSON_TYPE_OBJECT);
	if (c == 'a')
		return ((e->etype = SJSON_TYPE_ARRAY) == SJSON_TYPE_ARRAY);
	if (c == 's')
		return ((e->etype = SJSON_TYPE_STRING) == SJSON_TYPE_STRING);
	if (c == 'b')
		return ((e->etype = SJSON_TYPE_BOOLEAN) == SJSON_TYPE_BOOLEAN);
	if (c == 'r')
		return ((e->etype = SJSON_TYPE_REAL) == SJSON_TYPE_REAL);
	if (c == 'n')
		return ((e->etype = SJSON_TYPE_NULL) == SJSON_TYPE_NULL);
	if (c == 'v')
		return ((e->etype = SJSON_TYPE_NONE) == SJSON_TYPE_NONE);
	return (0);
}

int					sjson_explorer(const t_sjson_value *root,
									const char *form,
									...)
{
	va_list	ap;
	t_jae	e;
	int		pos;

	if (root == NULL || form == NULL)
		return (-1);
	va_start(ap, form);
	e = (t_jae){.node = NULL, .etype = SJSON_TYPE_NONE, .error_stack = 0,
		.valid = 0, .e = SJSON_ERROR_OK};
	pos = -1;
	while (form[++pos] != '\0')
		if (form[pos] == '$')
			e = (t_jae){.node = (t_sjson_value*)root, .etype = e.etype,
						.error_stack = 0, .valid = e.valid,
						.e = SJSON_ERROR_OK};
		else if (ft_isspace(form[pos]))
			;
		else if (!i_type_change(&e, form[pos]) && !i_access(&e, form[pos], ap))
		{
			va_end(ap);
			return (-1);
		}
	va_end(ap);
	return (e.valid);
}
