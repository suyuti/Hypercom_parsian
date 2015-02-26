/*******************************************************************************
*                   ----------------------------------------
*                    Copyright TEVERO AS, Norway, 2001-2003
*                                www.tevero.no
*                   ----------------------------------------
* This source code is the property of Tevero AS, Norway. The source/compiled
* code and derivatives may be used and distributed freely as long as this
* copyright notice is not removed.
*
* DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Tevero AS has no obligation to
* support this Software. Tevero AS is providing the Software "AS IS", with no
* express or implied warranties of any kind, including, but not limited to, any
* implied warranties of merchantability or fitness for any particular purpose
* or warranties against infringement of any proprietary rights of a third party.
* Tevero AS will not be liable for any consequential, incidental, or special
* damages, or any other relief, or for any claim by any third party, arising
* from your use of this Software.
*
*******************************************************************************/

/*******************************************************************************
********************************************************************************
********************************************************************************
*
* File:     fal.h
* Version:  1.7.1
* Descr.:   File Abstraction Layer include file. This include file contains
*           definitions for ANSI/ISO style buffered file operations. It acts
*           as an replacement for many normal file operations in order to
*           operate on flash disks.
*
* Author:   Tevero AS
* Date:     2001-01-10
*
********************************************************************************
********************************************************************************
*******************************************************************************/

#ifndef FAL_H
#define FAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Make sure <stdio.h> is included first, in order to be able to redefine */
/* some definitions. This avoids problems such that if an application file */
/* includes this file before including <stdio.h>. */
#include <stdio.h>

/*******************************************************************************
***********************             DEFINES              ***********************
*******************************************************************************/

/* Attributes when creating files. They should match similar */
/* TFFS_... defines, but we code them directly so that the user */
/* does not need to include TFFS include file. fal.c will make */
/* sure the defines are equal. */
#define FAL_READ              0x10
#define FAL_WRITE             0x20
#define FAL_HIDDEN            0x40
#define FAL_DIR               0x80

/* Access defines. They should match similar */
/* TFFS_... defines, but we code them directly so that the user */
/* does not need to include TFFS include file. fal.c will make */
/* sure the defines are equal. */
#define FAL_ACCESS_NORMAL     0x00
#define FAL_ACCESS_HIDDEN     0x40
#define FAL_ACCESS_SUPER      0x80

/* Monitor defines. They should match similar */
/* TFFS_... defines, but we code them directly so that the user */
/* does not need to include TFFS include file. fal.c will make */
/* sure the defines are equal. */
#define FAL_MON_OPEN_READ     0x00000001
#define FAL_MON_OPEN_WRITE    0x00000002
#define FAL_MON_OPEN_APPEND   0x00000004
#define FAL_MON_CLOSE_READ    0x00000008
#define FAL_MON_CLOSE_WRITE   0x00000010
#define FAL_MON_CLOSE_APPEND  0x00000020
#define FAL_MON_DELETE        0x00000040
#define FAL_MON_RENAME_FROM   0x00000080
#define FAL_MON_RENAME_TO     0x00000100

/* (Re)define needed file issues, in order to avoid conflict with */
/* <stdio.h>. We only (re)define the items used by FAL. */

/* End of File value for file stream. It should match similar */
/* TFFS_... define, but we code them directly so that the user */
/* does not need to include TFFS include file. fal.c will make */
/* sure the define is equal. */
#ifdef EOF
#undef EOF
#endif
#define EOF                   -1

#ifdef FILENAME_MAX
#undef FILENAME_MAX
#endif
#define FILENAME_MAX          28

/* File stream type */
#ifdef FILE
#undef FILE
#endif
#define FILE                  void

/* Seek positions */
#ifdef SEEK_SET
#undef SEEK_SET
#endif
#define SEEK_SET              0

#ifdef SEEK_CUR
#undef SEEK_CUR
#endif
#define SEEK_CUR              1

#ifdef SEEK_END
#undef SEEK_END
#endif
#define SEEK_END              2

#if 0

/* File functions */
#ifdef clearerr
#undef clearerr
#endif
#define clearerr              falClearerr

#ifdef fclose
#undef fclose
#endif
#define fclose                falFclose

#ifdef feof
#undef feof
#endif
#define feof                  falFeof

#ifdef ferror
#undef ferror
#endif
#define ferror                falFerror

#ifdef fflush
#undef fflush
#endif
#define fflush                falFflush

#ifdef fgetc
#undef fgetc
#endif
#define fgetc                 falFgetc

#ifdef fgets
#undef fgets
#endif
#define fgets                 falFgets

#ifdef fopen
#undef fopen
#endif
#define fopen                 falFopen

#ifdef fputc
#undef fputc
#endif
#define fputc                 falFputc

#ifdef fputs
#undef fputs
#endif
#define fputs                 falFputs

#ifdef fread
#undef fread
#endif
#define fread                 falFread

#ifdef fseek
#undef fseek
#endif
#define fseek                 falFseek

#ifdef ftell
#undef ftell
#endif
#define ftell                 falFtell

#ifdef fwrite
#undef fwrite
#endif
#define fwrite                falFwrite

#ifdef remove
#undef remove
#endif
#define remove                falRemove

#ifdef rename
#undef rename
#endif
#define rename                falRename

#ifdef rewind
#undef rewind
#endif
#define rewind                falRewind

/* (Re)define other FAL defined POSIX file issues, in order to avoid conflict */
/* with possibly include files. */

#ifdef chdir
#undef chdir
#endif
#define chdir                 falChdir

#ifdef getcwd
#undef getcwd
#endif
#define getcwd                falGetcwd

#ifdef rmdir
#undef rmdir
#endif
#define rmdir                 falRmdir

#endif

/*******************************************************************************
***********************            PROTOTYPES            ***********************
*******************************************************************************/

/* Misc. utility functions (non-standard) */
int falCp(const char *src, const char *dest);
long falFsize(FILE *stream);
unsigned char falGetAccessLevel(void *ptr);
int falGetAttributes(const char *file, unsigned char *attrib);
int falGetDefaultAttributes(unsigned char *attrib);
int falLs(const char *pathname,
          unsigned long entry,
          char *name,
          unsigned char *attr,
          unsigned long *size);
int falMkdir(const char *dir);
void *falMonitorAdd(const char *item,
                    unsigned long actions,
                    void (*callback)(void *callbackPtr, unsigned long action),
                    void *callbackPtr);
void falMonitorRemove(void *entry);
int falMount(int id, const char *name);
int falRegister(void);
int falSetAccessLevel(unsigned char level);
int falSetAttributes(const char *item, unsigned char attrib);
int falSetDefaultAttributes(unsigned char attrib);
void falUnmount(const char *name);
void falUnregister(void);


/* ANSI/ISO C like file operations */
void falClearerr(FILE *stream);
int falFclose(FILE *stream);
int falFeof(FILE *stream);
int falFerror(FILE *stream);
int falFflush(FILE *stream);
int falFgetc(FILE *stream);
char *falFgets(char *string, int n, FILE *stream);
FILE *falFopen(const char *filename, const char *mode);
int falFputc(int c, FILE *stream);
int falFputs(const char *string, FILE *stream);
size_t falFread(void *buf, size_t size, size_t count, FILE *stream);
int falFseek(FILE *stream, long offset, int origin);
long falFtell(FILE *stream);
size_t falFwrite(const void *buf, size_t size, size_t count, FILE *stream);
int falRemove(const char *item);
int falRename(const char *oldname, const char *newname);
void falRewind(FILE *stream);


/* POSIX like file operations */
int falChdir(const char *path);
int falFtruncate(FILE *stream, size_t length);
char *falGetcwd(char *buffer, size_t maxlen);
int falRmdir(const char *dir);
int falTruncate(const char *path, size_t length);

#ifdef __cplusplus
}
#endif

#endif  /* FAL_H */
