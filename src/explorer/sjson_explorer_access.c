/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_explorer_access.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/00/00 00:00:00 by hmartzol          #+#    #+#             */
/*   Updated: 2019/00/00 00:00:00 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sjson_defines.h>
#include <sjson_types.h>
#include <sjson_functions.h>

inline static int	i_store(t_jae *e, int do_store)
{
	void	*tmp;

	if (do_store)
		tmp = e->args[e->cur_arg++].ptr;
	if (!sjson_test_type(e->node, e->etype) || e->error_stack)
		return (1);
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

inline static int	i_right(t_jae *e)
{
	t_sjson_value	*tmp;

	if (e->error_stack)
		++e->error_stack;
	else if ((e->etype != SJSON_TYPE_OBJECT && e->etype != SJSON_TYPE_ARRAY)
			|| !sjson_test_type(e->node, e->etype))
		e->error_stack = 1;
	if (e->error_stack)
		return (++e->cur_arg || 1);
	if (e->etype == SJSON_TYPE_OBJECT)
	{
		if (sjson_search_pair_in_object(e->node, e->args[e->cur_arg++].ptr,
				&tmp) != SJSON_ERROR_OK)
			return (e->error_stack = 1);
		e->node = tmp;
		return (1);
	}
	if (sjson_search_index_in_array(e->node, e->args[e->cur_arg++].index, &tmp)
			!= SJSON_ERROR_OK)
		return (e->error_stack = 1);
	e->node = tmp;
	return (1);
}

inline static int	i_call(int common, t_jae *e)
{
	void				*arg;
	t_sjson_call_back	cb;

	cb = e->args[e->cur_arg++].ptr;
	if (common)
		arg = e->args[e->nb_arg].ptr;
	else
		arg = e->args[e->cur_arg++].ptr;
	if (e->error_stack || !sjson_test_type(e->node, e->etype))
		return (1);
	if (e->etype == SJSON_TYPE_NONE)
		e->e = cb(e->node, arg, e->node->type, e->key_index);
	else if (e->etype == SJSON_TYPE_STRING)
		e->e = cb(e->node->data.str, arg, e->node->type, e->key_index);
	else
		e->e = cb((void*)&e->node->data, arg, e->node->type, e->key_index);
	if (e->e == SJSON_ERROR_OK)
		++e->valid;
	return (1);
}

static inline int	i_length(t_jae *e)
{
	if (e->node->type == SJSON_TYPE_NONE)
	{
		++e->cur_arg;
		return (1);
	}
	if (e->node->type == SJSON_TYPE_ARRAY)
		*(t_sjson_size*)e->args[e->cur_arg++].ptr
			= e->node->data.ar.nb_values;
	else if (e->node->type == SJSON_TYPE_OBJECT)
		*(t_sjson_size*)e->args[e->cur_arg++].ptr
			= (t_sjson_size)e->node->data.obj.nb_groups;
	else if (e->node->type == SJSON_TYPE_STRING)
		*(t_sjson_size*)e->args[e->cur_arg++].ptr = e->node->data.str->length;
	else
		*(t_sjson_size*)e->args[e->cur_arg++].ptr = 1;
	++e->valid;
	return (1);
}

int					sjson_explorer_access(const char *form, int form_length,
	t_jae *e, int *i)
{
	if (form[*i] == '>')
		return (i_right(e));
	if (form[*i] == '<' && e->error_stack && --e->error_stack)
		return (1);
	if (form[*i] == '<')
		return ((e->root != e->node && (e->node = e->node->parent)) || 1);
	if (form[*i] == '*')
		return (i_store(e, 1));
	if (form[*i] == '#' || form[*i] == '@')
		return (i_call(form[*i] == '@', e));
	if (form[*i] == '~')
		return (i_length(e));
	if (form[*i] == '?')
		return (i_store(e, 0));
	if (form[*i] == '(')
		return (sjson_explorer_subscript(form, form_length, e, i));
	return (0);
}
