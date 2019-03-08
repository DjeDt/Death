#include <genetic.h>

void world_constructor(struct world *w);

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

bool code_generation(char *output UNUSED)
{
	struct world w;

	world_constructor(&w);

	for (unsigned char chromosome = 0; chromosome < POPULATION_SIZE; chromosome++)
	{
		printf("fitness: %d\n", w.c[chromosome].fitness);
		printf("chromosome: %s\n", w.c[chromosome].gene);
		printf("\n");
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// BOOTSTRAP FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
//	char code[MAX_GENE_SIZE];

	t_host host;

   	virtualize((uint8_t*)"---", 1, &host);

	printf("%ld\n", host.output);
	if (host.execution == false)
		printf("ret false %d\n", host.execution);
	else
		printf("ret true %d\n", host.execution);

//	code_generation(code);
}
