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



int		sjson_explorer_access(const char *form, int form_length, t_jae *e, int i)
{
	if (form[i] == '>')
		return (i_right(e));
	if (form[i] == '<' && e->error_stack && --e->error_stack)
		return (1);
	if (form[i] == '<')
		return ((e->node = e->node->parent) || 1);

}
