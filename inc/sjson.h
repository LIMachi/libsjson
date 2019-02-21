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

t_sjson_error	sjson_parse_str(char *src,
	t_sjson_value **out,
	t_sjson_flags flags,
	int fd_error);

t_sjson_error	sjson_parse_file(const char *path,
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
**   >: Go down a node
**     va_arg argumment:
**       on array: expect an index (t_sjson_size)
**       on object: expect a null terminated string (char*)
**   <: Go up a node
**   *: Store the current item
**     va_arg argumment:
**       object: t_sjson_object **
**       array: t_sjson_array **
**       node: t_sjson_value **
**       string: t_sjson_string **
**       null: void **
**       bool: t_sjson_bool *
**       real: t_sjson_real *
**   #: Will execute a function
**     va_arg argumments:
**       first argument: t_sjson_callback
**       second argument: void *
**     The function will recieve the current item (technically: a pointer to the
**     data of the node, if not the node itself) as the first argument, the
**     pointer passed in va_arg will be the second argument, the third is the
**     type of the first argument and the last argument is the key or iteration
**     (see subscript).
**     If no error where found, the callback must return SJSON_ERROR_OK
**     If errors where found, the callback must return a negative value
**     (suggested: SJSON_ERROR_KO)
**   @: Like #, but the second argument of the callback is the very last element
**     of the va_list (usefull for common structure data of callbacks)
**   ?: Just test if the value is valid (increment the return value like a valid
**     call to *, no argument is used)
**   ~: Store the length (number of subnodes of lists/objetcs or characters in
**     string), the pointer type should be t_sjson_size*, return 1 on other
**     types of nodes (NULL, BOOLEAN, REAL)
**   (subscript): Will execute the script enclosed in parenthesis for all
**     subnodes of the current list/object.
**     Each # and @ will receive in it's 4th argument the current iteration or
**     key, the function is responsible to knowing the type of it's 4th argument
**     (either *t_sjson_size, *char or NULL outside a sub-function)
**     To avoid undefined behavior $ will point to the node using the subscript.
**     A subscript can be used in another subscript recursively, but the 4th
**     argument of functions will be the iteration/key of the current
**     subscript, not a combination of the shilds-parents.
**     If the subscript does not encounter an error and validates at least one
**     # * ~ @ ? action, the sub-script will be counted as only ONE valid access
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

/*
** todo: use fprintf instead of dprintf for compatibility
*/

int				sjson_print(int fd,
							t_sjson_value *val,
							int flags);

void			sjson_free(t_sjson_value *value);

#endif
