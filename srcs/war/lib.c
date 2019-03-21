#include "war.h"

ssize_t	_get_random(void *buf, size_t buflen, unsigned int flags)
{
	ssize_t ret;

	__asm__ __volatile__ (
		"mov rdi, %0;"
		"mov rsi, %1;"
		"mov edx, %2;"
		"mov rax, 318;"
		"syscall" :: "g" (buf), "g" (buflen), "g" (flags)
		);

	__asm__ __volatile__ (
		"mov %0, rax;" : "=r" (ret)
		);
	return (ret);
}

int		_getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count)
{
	long ret;

	__asm__ __volatile__ (
		"mov edi, %0;"
		"mov rsi, %1;"
		"mov edx, %2;"
		"mov rax, 217;"
		"syscall" :: "g"(fd), "g"(dirp), "g"(count)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return ((int)ret);
}
