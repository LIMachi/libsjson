/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strchr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 19:13:08 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:23:27 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*std_strchr(char *s, int c)
{
	while (*s != '\0' && *s != c)
		++s;
	return (*s == c ? s : (char*)0);
}