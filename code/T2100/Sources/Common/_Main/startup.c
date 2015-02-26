
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             startup.c
//      
//
//=============================================================================

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#define HeapStart __bss_end__
#define HeapEnd __SDRAM_STACK_segment_start__

extern char HeapStart;
extern char HeapEnd;

typedef void (*void_fn)(void);
extern void_fn __ctors_start__;
extern void_fn __ctors_end__;
extern void_fn __dtors_start__;
extern void_fn __dtors_end__;

extern void main(int ac, char *av[]);
extern void __rt_entry( void );

void __rt_entry( void )
{
	void_fn *f;
	char *nothing[1] = { NULL };
	for(f = &__ctors_start__; f != &__ctors_end__; f++)
	{
		(*f)();
	}
	main(0, nothing);
	for(f = &__dtors_start__; f != &__dtors_end__; f++)
	{
		(*f)();
	}
}

void _exit( int n )
{
	for( n = 0;; )
	{
	}
}

caddr_t _sbrk( int incr )
{
	static char *heap_end;
	char *prev_heap_end;

	if( heap_end == NULL )
	{
		heap_end = &HeapStart;
	}

	prev_heap_end = heap_end;

	if( ( heap_end + incr ) > &HeapEnd )
	{
		errno = ENOMEM;
		return ( caddr_t )-1;
	}

	heap_end += incr;

	return ( caddr_t )prev_heap_end;
}

int isatty( int fd )
{
	( void )fd;
	return 0;
}

int _fstat( int fd, struct stat *st )
{
	( void )fd;
	memset( st, 0, sizeof( struct stat ) );
	//st->st_mode = 0100000;
	//st->st_blksize = 1024;
	return 0;
}

int _stat (const char *fname, struct stat *st)
{
  int file;

  /* The best we can do is try to open the file readonly.  If it exists,
     then we can guess a few things about it.  */
  if ((file = _open (fname, O_RDONLY)) < 0)
    return -1;

  memset (st, 0, sizeof (* st));
  st->st_mode = S_IFREG | S_IREAD;
  st->st_blksize = 1024;
  _close (file);
  return 0;
}
