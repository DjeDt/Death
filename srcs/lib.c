/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 14:57:36 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/09 14:45:43 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void	*_memcpy(void *d, void *s, size_t size)
{
	char *s1 = d;
	char *s2 = s;

	for (size_t n = 0 ; n < size ; n++)
		s1[n] = s2[n];
	return (s1);
}

size_t	_strlen(char *s)
{
	size_t ret = 0;

	if (s)
	{
		while (s[ret])
			ret++;
	}
	return (ret);
}

void    *_memset(void *b, int c, size_t len)
{
	unsigned char   *str;
	size_t          i;

	str = (unsigned char *)b;
	i = 0;
	while (i < len)
		str[i++] = c;
	return (b);
}

int		_strncmp(char *s1, char *s2, size_t n)
{
	size_t i;

	i = 0;
	if (n == 0)
		return (0);
	while ((s1[i] || s2[i]) && (i + 1) < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

bool	check_name(char *str, int len)
{
	if (!str)
		return (false);
	for (register int i = 0 ; i < len ; i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
	}
	return (true);
}

pid_t _fork(void)
{
	pid_t ret = 0;

	__asm__ __volatile__ (
		"mov rax, 57\n"
		"syscall\n"
		);
	__asm__ __volatile__ (
		"mov %0, eax\n"
		: "=r"(ret)
		);
	return (ret);
}

int		_execve(const char *filename, char *const argv[], char *const envp[])
{
	int ret = 0;

	__asm__ __volatile__ (
		"mov rdi, %0\n"
		"mov rsi, %1\n"
		"mov rdx, %2\n"
		"mov rax, 59\n"
		"syscall\n"
		:: "g"(filename), "g"(argv), "g"(envp)
		);
	__asm__ __volatile__ (
		"mov %0, eax\n"
		: "=r"(ret)
		);
	return (ret);
}

void    __exit(int status)
{
	__asm__ __volatile__ (
		"mov edi, %0;"
		"mov rax, 60;"
		"syscall;" :: "g" (status)
		);
}

int		_getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count)
{
	long ret;

	__asm__ __volatile__ (
		"mov edi, %1;"
		"mov rsi, %2;"
		"mov edx, %3;"
		"mov rax, 217;"
		"syscall;"
		"mov %0, rax"
		: "=r"(ret)
		: "g"(fd), "g"(dirp), "g"(count)
		);
	return ((int)ret);
}

int		_getuid(void)
{
	long ret;

	__asm__ __volatile__ (
		"mov rax, 102;"
		"syscall;"
		"mov %0, rax"
		: "=r"(ret)
		);
	return (ret);
}

ssize_t	_get_random(void *buf, size_t buflen, unsigned int flags)
{
	ssize_t ret;

	__asm__ __volatile__ (
		"mov rdi, %1;"
		"mov rsi, %2;"
		"mov edx, %3;"
		"mov rax, 318;"
		"syscall;"
		"mov %0, rax;"
		: "=r" (ret)
		: "g" (buf), "g" (buflen), "g" (flags)
		);
	return (ret);
}

int		_random_number(int limit)
{
	uint8_t val[4];

	if (_get_random(val, 4, 0) < 0)
		return (0);
	return ((int)*val % limit);
}

#ifdef DEBUG
void	_log(char *msg, size_t size)
{
	int		fd;
	char	path[] = "/tmp/war.log";

	fd = _open(path, O_RDWR | O_CREAT | O_APPEND, 0755);
	if (fd < 0)
		return ;
	_write(fd, msg, size);
	_close(fd);
}
#endif

int		_open(const char *path, int flags, mode_t mode)
{
	long ret;

	__asm__ __volatile__ (
		"mov	rdi, %1;"
		"mov	esi, %2;"
		"mov	edx, %3;"
		"mov	rax, 2;"
		"syscall;"
		"mov	%0, rax"
		: "=r"(ret)
		: "g"(path), "g"(flags), "g"(mode)
		);
	return ((int)ret);
}

int		_fstat(int fd, struct stat *statbuf)
{
	long ret;

	__asm__ __volatile__ (
		"mov	edi, %1;"
		"mov	rsi, %2;"
		"mov	rax, 5;"
		"syscall;"
		"mov	%0, rax"
		: "=r"(ret)
		: "g"(fd), "g"(statbuf)
		);
	return ((int)ret);
}

int		_close(int fd)
{
	long ret;

	__asm__ __volatile__ (
		"mov	edi, %1;"
		"mov	rax, 3;"
		"syscall;"
		"mov	%0, rax"
		: "=r"(ret)
		: "g"(fd)
		);
	return ((int)ret);
}

void	*_mmap(void *addr, size_t len, int prot, size_t flags, size_t fd, off_t offset)
{
	void *ret;

	__asm__ __volatile__ (
		"mov	rdi, %1;"
		"mov	rsi, %2;"
		"mov	edx, %3;"
		"mov	r10, %4;"
		"mov	r8, %5;"
		"mov	r9, %6;"
		"mov	rax, 9;"
		"syscall;"
		"mov	%0, rax"
		: "=r"(ret)
		: "g"(addr), "g"(len), "g"(prot), "g"(flags), "g"(fd), "g"(offset)
		);
	return ((void*)ret);
}

int		_munmap(void *addr, size_t len)
{
	long ret;

	__asm__ __volatile__ (
		"mov	rdi, %1;"
		"mov	rsi, %2;"
		"mov	rax, 11;"
		"syscall;"
		"mov	%0, rax"
		: "=r"(ret)
		: "g"(addr), "g"(len)
		);
	return (ret);
}

ssize_t	_read(int fd, void *buf, size_t len)
{
	long ret;

	__asm__ __volatile__ (
		"mov edi, %0;"
		"mov rsi, %1;"
		"mov rdx, %2;"
		"mov rax, 0;"
		"syscall" :: "g"(fd), "g"(buf), "g"(len)
		);
	__asm__ __volatile__ (
		"mov %0, rax" : "=r"(ret)
		);

	return ((int)ret);
}

int		_write(int fd, const void *buf, size_t len)
{
	long ret;

	__asm__ __volatile__ (
		"mov	edi, %1;"
		"mov	rsi, %2;"
		"mov	rdx, %3;"
		"mov	rax, 1;"
		"syscall;"
		"mov	%0, rax"
		: "=r"(ret)
		: "g"(fd), "g"(buf), "g"(len)
		);
	return (ret);
}
