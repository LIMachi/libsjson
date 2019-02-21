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

t_sjson_error	test_string_ok(t_sjson_string *str, t_sjson_arg_pair expected,
	t_sjson_value_type ignored1, t_sjson_size ingored2)
{
	size_t	p;

	(void)ignored1;
	(void)ingored2;
	printf("(call id: %d): fetching string %s\n", (int)expected.index,
		str->data);
	p = 0;
	while (str->data[p] != '\0' && ((char*)expected.ptr)[p] != '\0'
			&& str->data[p] == ((char*)expected.ptr)[p])
		++p;
	if (str->data[p] != ((char*)expected.ptr)[p])
		write(2, "unexpected string value\n", 24);
	return (SJSON_ERROR_OK);
}

int		main(void)
{
	t_sjson_value		*root;
	t_sjson_real		real;
	t_sjson_boolean		bol;
	t_sjson_size		*len;
	int					r;

	real = 42.0;
	bol = 0;
	r = 0;
	len = 0;
	printf("expected:\n{\n\t\"a\": [1, true],\n\t\"b\": \"ok\",\n\t\"c\": \"test2\"\n}\n");
	if (sjson_parse_file("test.json", &root, SJSON_FLAG_PRINT_ERRORS, 2)
			!= SJSON_ERROR_OK)
		return (write(2, "error while parsing\n", 20));
	write(1, "got:\n", 5);
	sjson_print(1, root, 0);
	write(1, "\n", 1);
	printf("void*: %d, size_t: %d, func: %d\n", (int)sizeof(void*),
		(int)sizeof(size_t), (int)sizeof(t_sjson_call_back));
	r = sjson_explorer(root, "$o>a>r*<a>b*$o>s#l*<o>s#",
		"a", (size_t)0, &real, (size_t)1, &bol,
		"b", test_string_ok, "ok", &len, "c", test_string_ok, "test2");
	printf("retrieved %d values\n", r);
	printf("real: %f\nboolean: %d\n", real, bol);
	printf("string length: %d\n", (int)len);
	sjson_free(root);
	return (0);
}
