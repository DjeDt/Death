/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   antidebug.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 13:37:18 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/10 23:59:13 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void	antidebug(t_data *data)
{

#ifdef DEBUG
	char de[] = "antidebug\t0\n";
	data->context == true ?	de[10] = 49 : 0;
	_log(de, _strlen(de));
#endif

	pid_t	child = 0;
	int		status = 0;

	if ((child = _fork()) < 0)
		__exit(0);
	if (child)
	{
		_wait4(child, &status, 0, NULL);
		if (WEXITSTATUS(status) == 1)
			__exit(1);
	}
	else
	{
		if (_ptrace(PTRACE_ATTACH, _getppid(), 0, 0) < 0)
		{
			char stap[] = "no debug plz\n";
			_write(1, stap, 13);
			__exit(1);
		}
		_wait4(_getppid(), NULL, 0, NULL);
		_ptrace(PTRACE_CONT, 0, 0, 0);
		_ptrace(PTRACE_DETACH, _getppid(), 0, 0);
		__exit(0);
	}


	/* update_two(&data->key, (char*)antidebug, (size_t)opening - (size_t)antidebug); */
	/* revert_two(&data->key, (char*)opening, (size_t)war - (size_t)opening); */
	opening(data);
}
