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
** stdarg.h
** va_list
*/

# include <stdarg.h>

/*
** typedef t_swt_map
*/

# include <ft_swiss_table.h>

typedef int						t_sjson_boolean;
typedef double					t_sjson_real;
typedef size_t					t_sjson_size;

typedef enum					e_sjson_error
{
	SJSON_ERROR_OK = 0,
	SJSON_ERROR_KO = -1,
	SJSON_ERROR_END_OF_FILE = -2,
	SJSON_ERROR_OUT_OF_MEMORY = -3,
	SJSON_ERROR_INVALID_SYNTAX = -4,
	SJSON_ERROR_INVALID_SEPARATOR = -5,
	SJSON_ERROR_INVALID_BOUND = -6,
	SJSON_ERROR_MISSING_ENDING_BOUND = -7,
	SJSON_ERROR_INVALID_PARAMETER = -8,
	SJSON_ERROR_MISMATCHED_NODE_TYPE = -9,
	SJSON_ERROR_TARGET_NOT_FOUND = -10,
	SJSON_ERROR_MISSING_COMMENT_ENDER = -11,
	SJSON_ERROR_FILE_ACCESS = -12
}								t_sjson_error;

typedef enum					e_sjson_flags
{
	SJSON_FLAG_NONE = 0,
	SJSON_FLAG_PRINT_ERRORS = 1
}								t_sjson_flags;

/*
** typedef struct s_sjson_object t_sjson_object;
*/

typedef t_swt_map				t_sjson_object;
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
	SJSON_TYPE_STRING = 0x40,
	SJSON_TYPE_LENGTH = 0x80,
	SJSON_TYPE_CALL_PAIR = 0x100,
	SJSON_TYPE_CALL_COMMON = 0x200
}								t_sjson_value_type;

typedef struct s_sjson_arg_pair		t_sjson_arg_pair;

struct								s_sjson_arg_pair
{
	void			*ptr;
	t_sjson_size	index;
};

typedef t_sjson_error			(*t_sjson_call_back)(void *node,
													t_sjson_arg_pair arg,
													t_sjson_value_type type,
													void *key_index);

struct							s_sjson_string
{
	t_sjson_size		length;
	char				data[0];
};

struct							s_sjson_pair
{
	t_sjson_string		*key;
	t_sjson_value		*value;
};

struct							s_sjson_object
{
	t_sjson_size		nb_pairs;
	t_sjson_pair		**pairs;
};

struct							s_sjson_array
{
	t_sjson_size		nb_values;
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

typedef union u_jae_arg		t_jae_arg;

typedef struct					s_jae
{
	t_sjson_value		*root;
	t_sjson_value		*node;
	t_sjson_value_type	etype;
	int					error_stack;
	int					valid;
	t_sjson_error		e;
	t_sjson_size		nb_arg;
	int					extra_arg;
	t_sjson_size		cur_arg;
	t_sjson_size		call_count;
	t_jae_arg			*args;
	void				*key_index;
}								t_jae;

union							u_jae_arg
{
	void				*ptr;
	t_sjson_size		index;
};

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
