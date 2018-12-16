/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 17:18:51 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 20:59:48 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SJSON_H
# define SJSON_H

# include "sjson_defines.h"
# include "sjson_types.h"
# include "sjson_functions.h"

/*
** Given the string src and it's length (facultative, set it to -1
** to ignore the length), extract a json tree stored in the pointer out
** (out will be allocated).
** The final parameter is used to pass flags (or-ed) to the parser:
**   0: no flags, default behavior
**   SJSON_FLAG_PRINT_ERRORS: will output detailed error information in stderr
** Will return SJSON_ERROR_OK if no errors (in src or allocations) where
** encountered.
** On error, it will try to fill as much as possible out before returning
** meaning on invalid syntax or truncated src, it will fill up to
** the last valid expression.
*/

t_sjson_error	sjson_parse_src(char *src,
								t_sjson_value **out,
								t_sjson_flags flags,
								int fd_error);

/*
** explorer to acces the content of nodes using an expression
** first argument: the node from which starts the exploration
** second argument: a format (described below)
**   $: root node
**   o: object cast
**   a: array cast
**   v: node cast
**   n: null cast
**   b: boolean cast
**   r: real cast
**   s: string cast
**   >: go down a node
**     va_arg argumment:
**       on array: expect an index (size_t)
**       on object: expect a null terminated string (unsigned char*)
**   <: go up a node
**   *: store the current item
**     va_arg argumment:
**       object: t_sjson_object **
**       array: t_sjson_array **
**       node: t_sjson_value **
**       string: t_sjson_string **
**       null: void **
**       bool: t_sjson_bool *
**       real: t_sjson_real *
**   #: will execute a function
**     va_arg argumments:
**       first argument: t_sjson_error (*)(void *, void*, t_sjson_value_type)
**       second argument: void *
**     The function will recieve the current item (technically: a pointer to the
**     data of the node, if not the node itself) as the first argument, the
**     pointer passed in va_arg will be the second argument and the last is the
**     type of the first argument.
**     If no error where found, the callback must return SJSON_ERROR_OK
** example:
**   file.json: {"test":["ok", null]}
**   t_sjson_error sjson_strcmp(t_sjson_string *strn, char *str)
**     {return (strcmp(strn->data, str));}
** given the above file and function declaration, the folowing call will succed
** (expect the root node to be an object, go down using the key "test", expect
** the node to be an array, go down using the index 0, expect the node to be a
** string, call sjson_strcmp on the node and the argument "ok")
**   sjson_explorer(root, "ro>a>s#", "test", 0, sjson_strcmp, "ok");
*/

int				sjson_explorer(const t_sjson_value *root,
								const char *form,
								...);

int				sjson_print(int fd,
							t_sjson_value *val,
							int flags);

#endif
