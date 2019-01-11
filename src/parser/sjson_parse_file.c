/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sjson_parse_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 19:08:18 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/11 19:04:26 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/sjson_defines.h"
#include "../../inc/sjson_types.h"
#include "../../inc/sjson_functions.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static inline t_sjson_error	i_finishing_touch(t_sjson_env *e,
	t_sjson_value **out,
	t_sjson_flags flags,
	int fd_error)
{
	t_sjson_error	err;

	e->limit = &e->src[e->slimit];
	e->pos = 0;
	e->fd_error = fd_error;
	e->flags = flags;
	(*out)->parent = *out;
	err = new_value(e, *out);
	free(e->src);
	return (err);
}

t_sjson_error				sjson_parse_file(const char *path,
	t_sjson_value **out,
	t_sjson_flags flags,
	int fd_error)
{
	t_sjson_env		e;
	struct stat		file_stat;
	int				fd;

	if (path == NULL || out == NULL)
		return (SJSON_ERROR_INVALID_PARAMETER);
	if (-1 == stat(path, &file_stat) || -1 == (fd = open(path, O_RDONLY)))
		return (SJSON_ERROR_FILE_ACCESS);
	if (NULL == (e.src = malloc(file_stat.st_size))
			|| NULL == (*out = malloc(sizeof(t_sjson_value))))
	{
		ft_clear("dp", fd, e.src);
		return (SJSON_ERROR_OUT_OF_MEMORY);
	}
	if ((size_t)-1 == (e.slimit = read(fd, e.src, file_stat.st_size)))
	{
		ft_clear("dpp", fd, e.src, out);
		return (SJSON_ERROR_FILE_ACCESS);
	}
	close(fd);
	e.path = (char*)path;
	return (i_finishing_touch(&e, out, flags, fd_error));
}
