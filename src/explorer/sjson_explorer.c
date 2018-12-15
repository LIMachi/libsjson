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

inline static int	sf_json_accesses_access_right(t_jae *e, va_list ap)
{
	t_sjson_value	*tmp;
	int				tmp1;
	char			*tmp2;

	if (e->error_stack)
		++e->error_stack;
	else if ((e->etype != object && e->etype != array) ||
			sjson_test_type(e->node, e->etype) != SJSON_ERROR_OK)
		e->error_stack = 1;
	if (e->error_stack)
		return (1 | va_arg(ap, size_t));
	if (e->etype == object)
	{
		if (sjson_search_pair_in_object(e->node,
				tmp2 = va_arg(ap, char *), &tmp) != SJSON_ERROR_OK)
			return (e->error_stack = 1);
		e->node = tmp;
		return (1);
	}
	if (sjson_search_index_in_array(e->node,
			tmp1 = va_arg(ap, int), &tmp) != SJSON_ERROR_OK)
		return (e->error_stack = 1);
	e->node = tmp;
	return (1);
}

inline static void	sf_json_accesses_access_store(t_jae *e, void *tmp)
{
	if (e->node->type == string)
		*(t_sjson_string**)tmp = e->node->data.str;
	if (e->node->type & null)
		*(void**)tmp = NULL;
	if (e->node->type & array)
		*(t_sjson_array**)tmp = &e->node->data.ar;
	if (e->node->type & object)
		*(t_sjson_object**)tmp = &e->node->data.obj;
	if (e->node->type == real)
		*(t_sjson_real*)tmp = e->node->data.real;
	if (e->node->type == boolean)
		*(t_sjson_boolean*)tmp = e->node->data.bol;
	if (e->node->type == none)
		*(t_sjson_value**)tmp = e->node;
}

inline static int	sf_json_accesses_access(t_jae *e, const char c, va_list ap)
{
	t_sjson_call_back	cb;

	if (c == '>')
		return (sf_json_accesses_access_right(e, ap));
	if (c == '<' && e->error_stack && --e->error_stack)
		return (1);
	if (c == '<')
	{
		e->node = e->node->parent;
		return (1);
	}
	cb = va_arg(ap, t_sjson_call_back);
	if (e->error_stack || (e->etype != none
			&& sjson_test_type(e->node, e->etype) != SJSON_ERROR_OK))
		return (1);
	if (c == '#')
		if (e->etype == none)
			cb(e->node, va_arg(ap, void*), e->node->type);
		else
			cb((void*)&e->node->data, va_arg(ap, void*), e->node->type);
	else
		sf_json_accesses_access_store(e, (void*)cb);
	return (1);
}

inline static int	sf_json_accesses_type_change(t_jae *e, const char c)
{
	if (c == 'o')
		return ((e->etype = object) == object);
	if (c == 'a')
		return ((e->etype = array) == array);
	if (c == 's')
		return ((e->etype = string) == string);
	if (c == 'b')
		return ((e->etype = boolean) == boolean);
	if (c == 'r')
		return ((e->etype = real) == real);
	if (c == 'n')
		return ((e->etype = null) == null);
	if (c == 'v')
		return ((e->etype = none) == none);
	if (c == '>' || c == '<' || c == '*' || c == '#')
		return (0);
	return (1);
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
	e = (t_jae){.node = NULL, .etype = none, .error_stack = 0};
	pos = -1;
	while (form[++pos] != '\0')
		if (form[pos] == '$')
			e = (t_jae){.node = (t_sjson_value*)root, .etype = e.etype,
						.error_stack = 0};
		else if (form[pos] == ' ')
			;
		else if (!sf_json_accesses_type_change(&e, form[pos]) &&
				!sf_json_accesses_access(&e, form[pos], ap))
		{
			va_end(ap);
			return (-1);
		}
	va_end(ap);
	return (0);
}
