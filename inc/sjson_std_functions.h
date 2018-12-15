/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_std_functions.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 19:11:25 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:28:30 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SJSON_STD_FUNCTIONS_H
# define SJSON_STD_FUNCTIONS_H

# include <stdlib.h>

int		std_strncmp(const char *s1, const char *s2, size_t n);
char	*std_strchr(char *s, int c);
void	*std_memcpy(void *dest, const void *src, size_t n);
int		std_isdigit(int c);

#endif