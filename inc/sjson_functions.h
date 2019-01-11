/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_functions.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 17:14:27 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 20:12:49 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SJSON_FUNCTIONS_H
# define SJSON_FUNCTIONS_H

# include "sjson_defines.h"
# include "sjson_types.h"

/*
** parser
*/

t_sjson_error	jump_blanks(t_sjson_env *e);

t_sjson_error	new_array(t_sjson_env *e,
						t_sjson_value *out);

t_sjson_error	new_object(t_sjson_env *e,
						t_sjson_value *out);

t_sjson_error	pair_extractor(t_sjson_env *e,
								t_sjson_pair *out,
								t_sjson_value *parent);

t_sjson_error	new_real(t_sjson_env *e,
						t_sjson_value *out);

t_sjson_error	in_set(t_sjson_env *e,
						char **set,
						size_t set_length,
						t_sjson_boolean update_pos);

t_sjson_error	new_value(t_sjson_env *e,
						t_sjson_value *out);

t_sjson_error	string_extractor(t_sjson_env *e,
								t_sjson_string **out);

/*
** explorer
*/

t_sjson_error	sjson_test_type(const t_sjson_value *value,
								const t_sjson_value_type type);

t_sjson_error	sjson_search_pair_in_object(t_sjson_value *v,
											const char *str,
											t_sjson_value **out);

t_sjson_error	sjson_search_index_in_array(t_sjson_value *value,
											size_t index,
											t_sjson_value **out);

/*
** print
*/

t_sjson_error	sjson_error(t_sjson_env *e, t_sjson_error err, char *func);

/*
** other
*/

void			*ft_clear(char *form, ...);

char			*sstrchr(char *str, int c);
int				sstrncmp(char *str1, char *str2, size_t n);
int				sstrcmp(char *str1, char *str2);
char			*sstrdup(char *str);
size_t			sstrlen(char *str);

int				sisdigit(int c);
int				sisspace(int c);

#endif
