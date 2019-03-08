#ifndef INTERPRETER_H
# define INTERPRETER_H

////////////////////////////////////////////////////////////////////////////////
/// INCLUDES
////////////////////////////////////////////////////////////////////////////////

# include <stdio.h>
# include <stdint.h>
# include <unistd.h>
# include <stdbool.h>

// extern module
# include "genetic.h"
# include "lib.h"


////////////////////////////////////////////////////////////////////////////////
/// MACROS
////////////////////////////////////////////////////////////////////////////////

// General
# define DEBUG

// Instructions
/* # define VAL_ADD		'+'	// *ptr++ */
/* # define VAL_SUB		'-'	// *ptr-- */
/* # define BEG_BCL		'[' // while { mem != 0 */
/* # define END_BCL		']' // end while */
/* # define INST_NUM		4 */

# define MEM_SIZE		1024

////////////////////////////////////////////////////////////////////////////////
/// ENUMS
////////////////////////////////////////////////////////////////////////////////

typedef enum state
{
	halt,
	run,
}			e_state;

typedef enum reg
{
	RAX,
	RBX,
	RCX,
	RDX,
	RSP,
	RBP,
	RSI,
	RDI,
	R8 ,
	R9 ,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15
}			e_register;

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

typedef struct		s_machine
{
	e_register		reg;
	e_state			state;
	size_t			memory[MEM_SIZE];
}					t_machine;

////////////////////////////////////////////////////////////////////////////////
/// PROTOTYPES
////////////////////////////////////////////////////////////////////////////////

void			virtualize(uint8_t *input, size_t len, t_host *c);
void			parse(t_machine *mach, t_host *host, uint8_t *input, size_t len);
void			execute(t_machine *mach, t_host *host, uint8_t *input, size_t len);

#endif
