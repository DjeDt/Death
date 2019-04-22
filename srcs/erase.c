/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   erase.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 13:20:56 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/22 20:35:27 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"


/*
  Erase() replace loaded binary data by random data.
  It the last idea we had to make the binary the most dificult to reverse.
  At least for a begginer reverser like us.
*/
void	erase(t_data *data)
{

	revert_one(&data->key, (char*)release, (size_t)erase - (size_t)release);

#ifdef DEBUG
	char log[] = "erase\t\t\t";
	_log(log, NULL,  7, data->context);
#endif

	char	*start = (char*)war;
	int		size = (size_t)release - (size_t)war;
	char	buf[size];

	// random or zeroed
	if (_get_random(buf, size, GRND_NONBLOCK) < 0)
	{
		for (register int i = 0 ; i < size ; i++)
			buf[i] = 0x0;
	}

	for (register int i = 0 ; i < size ; i++)
		start[i] = buf[i];

	end(data);
}
