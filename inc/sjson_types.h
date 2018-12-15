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
typedef int						t_sjson_error;
typedef int						t_sjson_flags;

typedef struct s_sjson_object	t_sjson_object;
typedef struct s_sjson_pair		t_sjson_pair;
typedef struct s_sjson_value	t_sjson_value;
typedef struct s_sjson_array	t_sjson_array;
typedef struct s_sjson_string	t_sjson_string;

typedef union u_sjson_data		t_sjson_data;

typedef enum					e_sjson_value_type
{
	invalid = 0x0,
	none = 0x1,
	null = 0x2,
	boolean = 0x4,
	array = 0x8,
	object = 0x10,
	real = 0x20,
	string = 0x40
}								t_sjson_value_type;

typedef void					(*t_sjson_call_back)(void *, void *,
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
	unsigned			error;
	t_sjson_data		data;
};

typedef struct					s_jae
{
	t_sjson_value		*node;
	t_sjson_value_type	etype;
	int					error_stack;
}								t_jae;

typedef struct					s_sjson_env
{
	unsigned char		*src;
	unsigned char		*limit;
	size_t				slimit;
	size_t				pos;
}								t_sjson_env;

#endif
