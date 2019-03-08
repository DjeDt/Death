#include <genetic.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void calculate_fitness(struct chromosome *c)
{
	(void)c;
}
/*
 	int thread[4];
	struct memory m[4];

	for (register unsigned char index = 0; index < 4; index++)
	{
		if ((m[index].stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == NULL)
		{
			perror("mmap");
			exit(1);
		}
	
		if ((thread[index] = clone(&worker, m[index].stack + STACK_SIZE, SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, NULL)) == -1)
		{
			perror("clone");
			exit(1);
		}
	}

	for (register unsigned char index = 0; index < 4; index++)
	{
		if (waitpid(thread[index], 0, 0) == -1)
		{
			perror("wait");
			exit(1);
		}
	
		if (munmap(m[index].stack, STACK_SIZE) == -1)
		{
			perror("munmap");
			exit(1);
		}
	}

	return 0;
}
*/
