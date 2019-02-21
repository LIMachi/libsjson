/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_explorer.c                                   :+:      :+:    :+:   */
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

inline static int	i_type_change(t_jae *e, const char c)
{
	if (c == 'o')
		return ((e->etype = SJSON_TYPE_OBJECT) || 1);
	if (c == 'a')
		return ((e->etype = SJSON_TYPE_ARRAY) || 1);
	if (c == 's')
		return ((e->etype = SJSON_TYPE_STRING) || 1);
	if (c == 'b')
		return ((e->etype = SJSON_TYPE_BOOLEAN) || 1);
	if (c == 'r')
		return ((e->etype = SJSON_TYPE_REAL) || 1);
	if (c == 'n')
		return ((e->etype = SJSON_TYPE_NULL) || 1);
	if (c == 'v')
		return ((e->etype = SJSON_TYPE_NONE) || 1);
	return (0);
}

int		sjson_explorer_internal(const char *form, int form_length, t_jae e)
{
	int	i;

	e.cur_arg = 0;
	e.etype = SJSON_TYPE_NONE;
	e.valid = 0;
	e.error_stack = 0;
	i = -1;
	while (++i < form_length)
		if (form[i] == '$')
		{
			e.node = e.root;
			e.error_stack = 0;
		}
		else if (sisspace(form[i]));
		else if (!i_type_change(&e, form[i])
			&& !sjson_explorer_access(form, form_length, &e, i))
		{
			free(e.args);
			return (SJSON_ERROR_INVALID_SYNTAX);
		}
	return (e.valid);
}

static inline int	sjson_explorer_prepare(const t_sjson_value *root,
	const char *form, t_jae *e)
{
	int		i;
	int		extra_arg;

	*e = (t_jae){.node = NULL, .cur_arg = 0, .args = NULL, .nb_arg = 0,
		.e = SJSON_ERROR_OK, .error_stack = 0, .etype = SJSON_TYPE_NONE,
		.valid = 0, .extra_arg = 0};
	if (root == NULL || form == NULL)
		return (SJSON_ERROR_INVALID_PARAMETER);
	i = -1;
	while (form[++i] != '\0')
		if (sstrchr("@*>~#", form[i]) != NULL)
		{
			if (form[i] == '@')
				e->extra_arg = 1;
			e->nb_arg += 1 + (form[i] == '#');
		}
	if ((e->args = malloc(sizeof(t_jae_arg) * (e->nb_arg + e->extra_arg)))
			== NULL)
		return (SJSON_ERROR_OUT_OF_MEMORY);
}

static inline int	sjson_explorer_load_args(const t_sjson_value *root,
	const char *form, t_jae *e, va_list *ap)
{
	int	i;

	i = -1;
	while (form[++i] != '\0')
		if (sstrchr("oasbrnv", form[i]) != NULL)
			i_type_change(e, form[i]);
		else if (sstrchr(">*#@~", form[i]) != NULL)
		{
			if (form[i] == '>')
				if (e->etype == SJSON_TYPE_ARRAY)
					e->args[e->cur_arg++].index = va_arg(*ap, t_sjson_size);
				else if (e->etype == SJSON_TYPE_OBJECT)
					e->args[e->cur_arg++].key = va_arg(*ap, char*);
				else
					return (SJSON_ERROR_INVALID_SYNTAX);
			else if (form[i] == '#')
			{
				e->args[e->cur_arg++].ptr = va_arg(*ap, void*);
				e->args[e->cur_arg++].ptr = va_arg(*ap, void*);
			}
			else
				e->args[e->cur_arg++].ptr = va_arg(*ap, void*);
		}
	return (SJSON_ERROR_OK);
}

int		sjson_explorer(const t_sjson_value *root, const char *form, ...)
{
	t_jae	e;
	va_list	ap;

	if ((e.e = sjson_explorer_prepare(root, form, &e)) != SJSON_ERROR_OK)
		return (e.e);

	va_start(ap, form);
	if ((e.e = sjson_explorer_load_args(root, form, &e, &ap)) != SJSON_ERROR_OK)
	{
		va_end(ap);
		free(e.args);
		return (e.e);
	}
	if (e.extra_arg)
		e.args[e.cur_arg].ptr = va_arg(ap, void*);
	va_end(ap);
	e.root = (t_sjson_value*)root;
	return (sjson_explorer_internal(form, (int)sstrlen(form), e));
}
