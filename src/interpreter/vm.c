#include "interpreter.h"

static void	init(t_machine *mach)
{
	mach->state = run;
	_bzero(mach->memory, MEM_SIZE);
}

void	virtualize(uint8_t *input, size_t len, t_host *host)
{
	t_machine	mach;

	init(&mach);
	printf("%d\n", mach.state);
	while (mach.state)
	{
		parse(&mach, host, input, len);
		execute(&mach, host, input, len);
		mach.state = halt;
	}
	printf("end\n");
	return ;
	/* c->execution = execute(&mach, input, len); */
	/* c->output = mach.memory; */
}
