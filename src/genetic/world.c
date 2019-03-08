#include <genetic.h>

int calculate_fitness(struct chromosome *c);

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void world_constructor(struct world *w)
{
	unsigned int gene_size;

	const char seed[] = {'<', '>', '+', '-', '[', ']'};
	const int seed_size = sizeof(seed);

	srand(time(NULL));

	w->calculate_fitness = &calculate_fitness;

	for (register unsigned int chromosome = 0; chromosome < POPULATION_SIZE; chromosome++)
	{
		w->c[chromosome].fitness = 0;
		bzero((void *)(w->c[chromosome].gene), MAX_GENE_SIZE);
		gene_size = RANDOM_INTEGER(MAX_GENE_SIZE);

		for (register unsigned int gene = 0; gene < gene_size; gene++)
		{
			w->c[chromosome].gene[gene] = seed[RANDOM_INTEGER(seed_size)];
		}
	}
}
