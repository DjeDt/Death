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

void	end_lib(void)
{
	return ;
}
