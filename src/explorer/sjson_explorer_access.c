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

static inline t_sjson_size	i_length(t_sjson_value *node)
{
	if (node->type == SJSON_TYPE_NONE)
		return (0);
	if (node->type == SJSON_TYPE_ARRAY)
		return (node->data.ar.nb_values);
	else if (node->type == SJSON_TYPE_OBJECT)
		return ((t_sjson_size)node->data.obj.nb_groups);
	else if (node->type == SJSON_TYPE_STRING)
		return (node->data.str->length);
	return (1);
}

inline static int	i_store(t_jae *e, int do_store)
{
	void	*tmp;

	if (do_store)
		tmp = e->args[e->cur_arg++].ptr;
	if (!do_store || (e->etype != SJSON_TYPE_LENGTH
			&& !sjson_test_type(e->node, e->etype)) || e->error_stack)
		return (1);
	if (e->etype == SJSON_TYPE_LENGTH)
		*(t_sjson_size *)tmp = i_length(e->node);
	else if (e->node->type == SJSON_TYPE_STRING)
		*(t_sjson_string **)tmp = e->node->data.str;
	else if (e->node->type & SJSON_TYPE_NULL)
		*(void **)tmp = NULL;
	else if (e->node->type & SJSON_TYPE_ARRAY)
		*(t_sjson_array **)tmp = &e->node->data.ar;
	else if (e->node->type & SJSON_TYPE_OBJECT)
		*(t_sjson_object **)tmp = &e->node->data.obj;
	else if (e->node->type == SJSON_TYPE_REAL)
		*(t_sjson_real *)tmp = e->node->data.real;
	else if (e->node->type == SJSON_TYPE_BOOLEAN)
		*(t_sjson_boolean *)tmp = e->node->data.bol;
	else if (e->node->type == SJSON_TYPE_NONE)
		*(t_sjson_value **)tmp = e->node;
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
	t_sjson_arg_pair	arg_pair;
	t_sjson_call_back	cb;
	t_sjson_size		tmp_size;

	cb = e->args[e->cur_arg++].ptr;
	arg_pair = (t_sjson_arg_pair){.ptr = common ? e->args[e->nb_arg].ptr
							: e->args[e->cur_arg++].ptr, e->call_count++};
	if (e->error_stack || (e->etype != SJSON_TYPE_LENGTH
						&& !sjson_test_type(e->node, e->etype)))
		return (1);
	if (e->etype == SJSON_TYPE_LENGTH && (tmp_size = i_length(e->node)))
		e->e = cb(&tmp_size, arg_pair, SJSON_TYPE_LENGTH, e->key_index);
	else if (e->etype == SJSON_TYPE_NONE)
		e->e = cb(e->node, arg_pair, e->node->type, e->key_index);
	else if (e->etype == SJSON_TYPE_STRING)
		e->e = cb(e->node->data.str, arg_pair, e->node->type, e->key_index);
	else
		e->e = cb((void*)&e->node->data, arg_pair, e->node->type, e->key_index);
	if (e->e == SJSON_ERROR_OK)
		++e->valid;
	else
		e->e = e->e == SJSON_ERROR_KO ? SJSON_ERROR_OK : e->e;
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
	if (form[*i] == '*' || form[*i] == '?')
		return (i_store(e, form[*i] == '*'));
	if (form[*i] == '#' || form[*i] == '@')
		return (i_call(form[*i] == '@', e));
	if (form[*i] == '(')
		return (sjson_explorer_subscript(form, form_length, e, i));
	return (0);
}
