/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_types.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 11:01:52 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 20:02:07 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SJSON_TYPES_H
# define SJSON_TYPES_H

/*
** stdlib.h
** typedef size_t
** #define NULL
*/

# include <stdlib.h>

typedef int						t_sjson_boolean;
typedef double					t_sjson_real;

typedef enum					e_sjson_error
{
	SJSON_ERROR_KO = -1,
	SJSON_ERROR_OK = 0,
	SJSON_ERROR_END_OF_FILE = 1,
	SJSON_ERROR_OUT_OF_MEMORY = 2,
	SJSON_ERROR_INVALID_SYNTAX = 3,
	SJSON_ERROR_INVALID_SEPARATOR = 4,
	SJSON_ERROR_INVALID_BOUND = 5,
	SJSON_ERROR_MISSING_ENDING_BOUND = 6,
	SJSON_ERROR_INVALID_PARAMETER = 7,
	SJSON_ERROR_MISMATCHED_NODE_TYPE = 8,
	SJSON_ERROR_TARGET_NOT_FOUND = 9,
	SJSON_ERROR_MISSING_COMMENT_ENDER = 10,
	SJSON_ERROR_FILE_ACCESS = 11
}								t_sjson_error;

typedef enum					e_sjson_flags
{
	SJSON_FLAG_NONE = 0,
	SJSON_FLAG_PRINT_ERRORS = 1
}								t_sjson_flags;

typedef struct s_sjson_object	t_sjson_object;
typedef struct s_sjson_pair		t_sjson_pair;
typedef struct s_sjson_value	t_sjson_value;
typedef struct s_sjson_array	t_sjson_array;
typedef struct s_sjson_string	t_sjson_string;

typedef union u_sjson_data		t_sjson_data;

typedef enum					e_sjson_value_type
{
	SJSON_TYPE_INVALID = 0x0,
	SJSON_TYPE_NONE = 0x1,
	SJSON_TYPE_NULL = 0x2,
	SJSON_TYPE_BOOLEAN = 0x4,
	SJSON_TYPE_ARRAY = 0x8,
	SJSON_TYPE_OBJECT = 0x10,
	SJSON_TYPE_REAL = 0x20,
	SJSON_TYPE_STRING = 0x40
}								t_sjson_value_type;

typedef t_sjson_error			(*t_sjson_call_back)(void *, void *,
													t_sjson_value_type);

struct							s_sjson_string
{
	size_t				length;
	char				data[0];
};

struct							s_sjson_pair
{
	t_sjson_string		*key;
	t_sjson_value		*value;
};

struct							s_sjson_object
{
	size_t				nb_pairs;
	t_sjson_pair		**pairs;
};

struct							s_sjson_array
{
	size_t				nb_values;
	t_sjson_value		**values;
};

union							u_sjson_data
{
	t_sjson_array		ar;
	t_sjson_object		obj;
	t_sjson_boolean		bol;
	t_sjson_real		real;
	t_sjson_string		*str;
};

struct							s_sjson_value
{
	t_sjson_value		*parent;
	t_sjson_value_type	type;
	t_sjson_error		error;
	t_sjson_data		data;
};

typedef struct					s_jae
{
	t_sjson_value		*node;
	t_sjson_value_type	etype;
	int					error_stack;
	int					valid;
	t_sjson_error		e;
}								t_jae;

typedef struct					s_sjson_env
{
	char				*src;
	char				*path;
	char				*limit;
	size_t				slimit;
	size_t				pos;
	t_sjson_flags		flags;
	int					fd_error;
}								t_sjson_env;

#endif
