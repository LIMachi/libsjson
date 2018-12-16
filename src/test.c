/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 20:04:12 by hmartzol          #+#    #+#             */
/*   Updated: 2018/10/12 16:45:09 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sjson.h>

#include <unistd.h>

#include <string.h>

void	test_string_ok(char *str, char *expected)
{
	size_t	p;

	p = 0;
	while (str[p] != '\0' && expected[p] != '\0'
			&& str[p] == expected[p])
		++p;
	if (str[p] != expected[p])
		write(2, "unexpected SJSON_TYPE_STRING value\n", 24);
}

int		main(const int argc, const char **argv)
{
	t_sjson_value		*root;
	t_sjson_real		real;
	t_sjson_boolean		bol;

	write(1, "expected:\n{\n\t\"a\": [1, true],\n\t\"b\": \"ok\"\n}\n", 42);
	if (argc != 2)
	{
		write(1, "at least try to give some argumment :)\n", 39);
		return (0);
	}
	if (sjson_parse_src((char*)argv[1], &root, SJSON_FLAG_PRINT_ERRORS, 2)
			!= SJSON_ERROR_OK)
		write(2, "error while parsing\n", 20);
	write(1, "got:\n", 5);
	sjson_print(1, root, 0);
	write(1, "\n", 1);
	if (sjson_explorer(root, "$o>a>r*<a>b*$o>s#",
		"a", 0, &real, 1, &bol,
		"b", test_string_ok, "ok"))
		write(2, "error on accesses\n", 18);
	if (real != 1)
		write(2, "unexpected int value\n", 21);
	if (bol != 1)
		write(2, "unexpected bool value\n", 22);
	return (0);
}
