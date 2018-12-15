/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_real.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/29 17:34:48 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:36:46 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <sjson_defines.h>
// #include <sjson_types.h>

# include "../../inc/sjson_defines.h"
# include "../../inc/sjson_types.h"
# include "../../inc/sjson_std_functions.h"

#if SJSON_EXTEND

t_sjson_error	new_real(t_sjson_env *e,
						t_sjson_value *out)
{
	unsigned char	tc;
	unsigned char	*tmp;

	tc = *e->limit;
	*e->limit = '\0';
	out->data.real = strtod(&e->src[e->pos], &tmp);
	e->pos = (size_t)(tmp - &e->src[e->pos]);
	*e->limit = tc;
	return (SJSON_ERROR_OK);
}

#else

t_sjson_error	new_real_exponent(t_sjson_env *e,
							t_sjson_value *out,
							t_sjson_real sign,
							t_sjson_real tmp)
{
	int	esign;
	int ex;

	if (++e->pos >= e->slimit)
		return (SJSON_ERROR_EOF);
	esign = 0;
	if (e->src[e->pos] == '-' || e->src[e->pos] == '+')
		esign = e->src[e->pos++] == '-';
	ex = 0;
	if (e->pos >= e->slimit)
		return (SJSON_ERROR_EOF);
	if (!std_isdigit(e->src[e->pos]))
		return (SJSON_INVALID_SYNTAX);
	while (e->pos < e->slimit && std_isdigit(e->src[e->pos]))
		ex = ex * 10 + e->src[e->pos++] - '0';
	if (esign)
		while (ex--)
			tmp /= 10.0;
	else
		while (ex--)
			tmp *= 10.0;
	out->data.real = tmp * sign;
	return (SJSON_ERROR_OK);
}

t_sjson_error	new_real_2(t_sjson_env *e,
							t_sjson_value *out,
							t_sjson_real sign,
							t_sjson_real tmp)
{
	t_sjson_real	tmp2;

	if (e->pos >= e->slimit)
		return (SJSON_ERROR_OK);
	if (e->src[e->pos] == '.')
	{
		if (++e->pos >= e->slimit)
			return (SJSON_ERROR_EOF);
		if (!std_isdigit(e->src[e->pos]))
			return (SJSON_INVALID_SYNTAX);
		tmp2 = 1.0;
		while (e->pos < e->slimit && std_isdigit(e->src[e->pos]))
		{
			tmp = tmp * 10.0 + (t_sjson_real)(e->src[e->pos++] - '0');
			tmp2 *= 10.0;
		}
		tmp /= tmp2;
	}
	if (e->pos < e->slimit && (e->src[e->pos] == 'e' || e->src[e->pos] == 'E'))
		return (new_real_exponent(e, out, sign, tmp));
	out->data.real = tmp * sign;
	return (SJSON_ERROR_OK);
}

t_sjson_error	new_real(t_sjson_env *e,
						t_sjson_value *out)
{
	t_sjson_real	sign;
	t_sjson_real	tmp;

	if (e->pos < e->slimit && e->src[e->pos] == '-')
	{
		++e->pos;
		sign = -1.0;
	}
	else
		sign = 1.0;
	if (e->pos >= e->slimit)
		return (SJSON_ERROR_EOF);
	if (e->src[e->pos] == '0')
	{
		tmp = 0.0;
		++e->pos;
	}
	else if (std_isdigit(e->src[e->pos]) && !(tmp = 0))
		while (e->pos < e->slimit && std_isdigit(e->src[e->pos]))
			tmp = tmp * 10.0 + (t_sjson_real)(e->src[e->pos++] - '0');
	else
		return (SJSON_INVALID_SYNTAX);
	return (new_real_2(e, out, sign, tmp));
}

#endif
