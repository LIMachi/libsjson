/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_explorer_subscript.c                         :+:      :+:    :+:   */
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

static inline void	i_object(const char *form, int form_length, t_jae *e,
							t_jae se)
{
	t_jae			le;
	size_t			it;
	int				valid;

	it = 0;
	valid = 0;
	se.node = se.root;
	se.e = SJSON_ERROR_OK;
	se.error_stack = 0;
	while (ft_swiss_table_iterate(&e->node->data.obj, &it, &se.key_index,
			(void**)&se.root) == 1)
	{
		le = se;
		if (sjson_explorer_internal(form, form_length, &le) > 1)
			valid = 1;
	}
	e->cur_arg += le.cur_arg;
	if (valid)
		++e->valid;
}

static inline void	i_array(const char *form, int form_length, t_jae *e,
							t_jae se)
{
	t_sjson_size	i;
	t_jae			le;
	int				valid;

	i = (t_sjson_size)-1;
	se.key_index = (void*)&i;
	se.node = se.root;
	se.e = SJSON_ERROR_OK;
	valid = 0;
	while (++i < e->node->data.ar.nb_values)
	{
		le = se;
		le.root = e->node->data.ar.values[i];
		if (sjson_explorer_internal(form, form_length, &le) > 1)
			valid = 1;
	}
	e->cur_arg += le.cur_arg;
	if (valid)
		++e->valid;
}

static inline void	i_jump_unused_args(const char *form, int form_length,
	t_jae *e, int error)
{
	int	i;
	int	c;

	i = -1;
	c = 0;
	while (++i < form_length)
		if (sstrchr("@*>#", form[i]) != NULL)
			c += 1 + (form[i] == '#');
	e->cur_arg += c;
	e->e = error ? SJSON_ERROR_INVALID_SYNTAX : SJSON_ERROR_OK;
}

int					sjson_explorer_subscript(const char *form, int form_length,
											t_jae *e, int *i)
{
	t_jae	se;
	int		fp;
	int		s;

	se = *e;
	se.nb_arg -= e->cur_arg;
	se.args = &e->args[e->cur_arg];
	se.root = e->node;
	fp = *i;
	s = 1;
	while (s && ++fp < form_length)
		s += (form[fp] == '(') - (form[fp] == ')');
	if (s)
		return (e->e = SJSON_ERROR_MISSING_ENDING_BOUND);
	if (!e->error_stack && sjson_test_type(e->node, e->etype))
		if (e->etype == SJSON_TYPE_OBJECT)
			i_object(&form[*i + 1], fp - *i - 1, e, se);
		else if (e->etype == SJSON_TYPE_ARRAY)
			i_array(&form[*i + 1], fp - *i - 1, e, se);
		else
			i_jump_unused_args(&form[*i + 1], fp - *i - 1, e, 1);
	else
		i_jump_unused_args(&form[*i + 1], fp - *i - 1, e, 0);
	*i = fp;
	return (1);
}
