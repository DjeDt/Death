#include "interpreter.h"

void	*_bzero(void *data, size_t size)
{
	unsigned char *tmp;

	tmp = data;
	while (size--)
		*tmp++ = 0;
	return (data);
}

size_t	_strlen(const char *s)
{
	size_t len = 0;

	while (len[s])
		len++;
	return (len);
}
