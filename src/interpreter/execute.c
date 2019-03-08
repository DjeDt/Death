#include "interpreter.h"

static void	get_inst(t_machine *mach, t_host *host, uint8_t *input, size_t len)
{
	(void)mach;
	(void)host;
	(void)input;
	(void)len;
}

void		execute(t_machine *mach, t_host *host, uint8_t *input, size_t len)
{
	printf("execute\n");

	uint64_t inst; (void)inst;

	for (size_t i = 0 ; i < len ; i++)
	{
		get_inst(mach, host, input, i);
		// execution
	}
	host->execution = true;
	host->output = 0;
	mach->state = halt;
	return ;
}
