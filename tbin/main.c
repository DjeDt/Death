#include <stdio.h>

int main(int ac, char **av)
{
	if (ac == 2)
		puts(av[1]);
	else
		puts("error -> usage: bin [arg]");
	return (0);
}
