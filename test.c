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

#include <stdio.h>

void	test_string_ok(t_sjson_string *str, char *expected)
{
	size_t	p;

	printf("fetching string %s\n", str->data);
	p = 0;
	while (str->data[p] != '\0' && expected[p] != '\0'
			&& str->data[p] == expected[p])
		++p;
	if (str->data[p] != expected[p])
		write(2, "unexpected string value\n", 24);
}

int		main(void)
{
	t_sjson_value		*root;
	t_sjson_real		real;
	t_sjson_boolean		bol;
	int					r;

	real = 42.0;
	bol = 0;
	write(1, "expected:\n{\n\t\"a\": [1, true],\n\t\"b\": \"ok\"\n}\n", 42);
	if (sjson_parse_file("test.json", &root, SJSON_FLAG_PRINT_ERRORS, 2)
			!= SJSON_ERROR_OK)
		return (write(2, "error while parsing\n", 20));
	write(1, "got:\n", 5);
	sjson_print(1, root, 0);
	write(1, "\n", 1);
	printf("void*: %d, size_t: %d, func: %d\n", (int)sizeof(void*),
		(int)sizeof(size_t), (int)sizeof(t_sjson_call_back));
	r = sjson_explorer(root, "$o>a>r*<a>b*$o>s#",
		"a", (size_t)0, &real, (size_t)1, &bol,
		"b", test_string_ok, "ok");
	printf("retrieved %d values\n", r);
	printf("real: %f\nboolean: %d\n", real, bol);
	sjson_free(root);
	return (0);
}
