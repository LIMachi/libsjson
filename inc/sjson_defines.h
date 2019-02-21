/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_defines.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 10:55:54 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:03:52 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SJSON_DEFINES_H
# define SJSON_DEFINES_H

# ifndef SJSON_EXTEND
#  define SJSON_EXTEND 0
# endif

# if SJSON_EXTEND

#  define SJSON_STRING_BOUNDS "\"'"

#  define SJSON_PAIR_SEPARATORS ":="

# else

#  define SJSON_STRING_BOUNDS "\""

#  define SJSON_PAIR_SEPARATORS ":"

# endif

# define SJSON_ARRAY_SEPARATORS ","

# define SJSON_OBJECT_COUNT 1
# define SJSON_OBJECT_STARTERS ((char*[SJSON_OBJECT_COUNT]){"{"})
# define SJSON_OBJECT_ENDERS ((char*[SJSON_OBJECT_COUNT]){"}"})

# define SJSON_ARRAY_COUNT 1
# define SJSON_ARRAY_STARTERS ((char*[SJSON_ARRAY_COUNT]){"["})
# define SJSON_ARRAY_ENDERS ((char*[SJSON_ARRAY_COUNT]){"]"})

# define SJSON_BLANKS " \t\n\r\v"

# define SJSON_NULL_COUNT 1
# define SJSON_NULL_SET ((char*[SJSON_NULL_COUNT]){"null"})

# define SJSON_TRUE_COUNT 1
# define SJSON_TRUE_SET ((char*[SJSON_TRUE_COUNT]){"true"})

# define SJSON_FALSE_COUNT 1
# define SJSON_FALSE_SET ((char*[SJSON_FALSE_COUNT]){"false"})

# define SJSON_STARTING_ARRAY_SIZE 16
# define SJSON_STARTING_OBJECT_SIZE 16

#endif
