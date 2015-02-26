/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
#ifndef _USEVHDTFILESYS_H
#define _USEVHDTFILESYS_H

#if defined(MAKE_OPTIMUM)

#if defined(__cplusplus)
	extern "C" {
#endif

#if !defined( MNT )

extern FILE * vhdt_fopen( const char *filename, const char *mode );
extern size_t vhdt_fwrite( const void *ptr, size_t elemsz, size_t nelem, FILE *stream );
extern size_t vhdt_fread( void *ptr, size_t elemsz, size_t nelem, FILE *stream );
extern int vhdt_fputc( int c, FILE *stream );
extern int vhdt_fputs( const char *s, FILE *stream );
extern int vhdt_remove( const char *filename );
extern int vhdt_rename( const char *oldname, const char *newname );
extern int vhdt_fseek( FILE *, long offset, int origin );
extern int vhdt_fclose( FILE *stream );
extern int vhdt_fflush( FILE *stream );
extern int vhdt_ferror( FILE *stream );

#if !defined(_MSC_VER)	// Visual C++
extern int vhdt_fgetc( FILE *stream );
extern void vhdt_rewind( FILE *stream );
#endif // !defined(_MSC_VER)

extern int vhdt_ftell( FILE *stream );
extern int vhdt_feof( FILE *stream );

#ifndef HFS_SIMULATION

	#define fopen	vhdt_fopen
	#define fwrite	vhdt_fwrite
	#define fputc	vhdt_fputc
	#define fputs	vhdt_fputs
	#define remove	vhdt_remove
	#define rename	vhdt_rename
	#define fread	vhdt_fread
	#define fseek   vhdt_fseek
	#define fclose	vhdt_fclose
	#define fflush	vhdt_fflush

	#undef ferror
	#define ferror	vhdt_ferror

#if !defined( _MSC_VER )	// Visual C++
	#define fgetc	vhdt_fgetc
	#define rewind	vhdt_rewind
#endif // !defined(_MSC_VER)

	#define ftell	vhdt_ftell
	#undef feof
	#define feof	vhdt_feof

#endif // HFS_SIMULATION

#endif // MNT

#if defined(__cplusplus)
	}
#endif

#elif defined(MAKE_VIKING)

#if !defined(__cplusplus)

extern FILE * vhdt_fopen( const char *filename, const char *mode );
extern size_t vhdt_fwrite( const void *ptr, size_t elemsz, size_t nelem, FILE *stream );
extern size_t vhdt_fread( void *ptr, size_t elemsz, size_t nelem, FILE *stream );
extern int vhdt_fputc( int c, FILE *stream );
extern int vhdt_fputs( const char *s, FILE *stream );
extern int vhdt_remove( const char *filename );
extern int vhdt_rename( const char *old, const char *new );
extern int vhdt_fseek( FILE *, long offset, int origin );
extern int vhdt_fclose( FILE *stream );

#ifndef HFS_SIMULATION

	#define fopen	vhdt_fopen
	#define fwrite	vhdt_fwrite
	#define fputc	vhdt_fputc
	#define fputs	vhdt_fputs
	#define remove	vhdt_remove
	#define rename	vhdt_rename
	#define fread	vhdt_fread
	#define fseek   vhdt_fseek
	#define fclose	vhdt_fclose

#endif // HFS_SIMULATION

#endif // __cplusplus


#endif

#endif // _USEVHDTFILESYS_H

