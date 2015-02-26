/*******************************************************************************
*                   ----------------------------------------
*                    Copyright TEVERO AS, Norway, 2000-2003
*                                www.tevero.no
*                   ----------------------------------------
* This source code is the property of Tevero AS, Norway. The source/compiled
* code may not be used or modified without a written consent from Tevero AS.
* The consent shall specify under which conditions the code may be used.
*
* This copyright notice may not be removed from the source code nor changed.
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
* File:     shrdconf.h
* Version:  1.1.2
* Descr.:   Shared Configuration include file used in order to customize shared
*           target/user specific definitions. This file is typically modified
*           per target in order to make the system work in the desired
*           environment. Notice that subproducts may have their own
*           configuration file, but they will in most cases make use of
*           this file in addition.
*
* Author:   Tevero AS
* Date:     2002-08-06
*
********************************************************************************
********************************************************************************
*******************************************************************************/

#ifndef SHRDCONF_H
#define SHRDCONF_H

/*******************************************************************************
                             TARGET INCLUDE FILES
 *******************************************************************************
 Add include files here if it is needed due to customized defines in this file
 or for possible other reasons. These include files will be included in all
 subproduct source code when compiling. Typical need would for instance
 be to add standard C include files, due to using standard C glue functions
 defined below.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>



/*******************************************************************************
                             BASIC DATA TYPES
 *******************************************************************************
 Define basic data type in order to make system less dependent on CPU and
 compiler. Make sure that the below defines matches the environment in use.
*******************************************************************************/

#ifndef U8
#define U8 unsigned char              /* Unsigned 8 bit */
#endif

#ifndef U16
#define U16 unsigned short int        /* Unsigned 16 bit */
#endif

#ifndef U32
#define U32 unsigned int              /* Unsigned 32 bit */
#endif

#ifndef size_t
#define size_t U32                    /* Unsigned 32 bit */
#endif

/*******************************************************************************
                                MAX PATH
 *******************************************************************************
 Define maximum number of characters in path including filename. Notice that
 every user registered as a disk user will have allocated a character array of
 this size (+ 1 terminating character), in order to change directory etc.

 In some rare cases, temp buffers of this size may also be allocated in order
 to handle path manipulation, rathere than using the stack for such issues.
*******************************************************************************/

#define SHRD_MAX_PATH                 200



/*******************************************************************************
                                ERROR NUMBERING
 *******************************************************************************

 ---------------------------------- NOTICE: ------------------------------------
 errno is normally in full scale systems set on an error in a system-level call.
 In the context of this source code, it will be set in most system call
 comparable functions, such as opening a file, reading from a file etc. 'errno'
 should ONLY be used in order to obtain more information on why a function call
 failed, NOT whether it failed or not. Support is included for the following
 main reasons:

 - Improve debug support
 - Make it possible to use application source including use of 'errno'

 'errno' reflects the latest occurring fault, it is NEVER cleared by this SW.

 Please refer to individual function documentation whether 'errno' is updated
 or not.
 -------------------------------------------------------------------------------

 Define how to use 'errno' reporting:

 0 - The 'errno' variable is not modified.
 1 - The 'errno' variable will be updated indicating cause of failure in
     many function calls (but not all).
 2 - As for 1, but in addition every modification to 'errno' is printed. This
     is intended for debugging and requires the presence of strerror() and
     that the shrdPrintf() macro below is defined.
 
 The proper errno.h include file must also be defined below if 'errno' updating
 is enabled, since the actual 'errno' values may differ for different systems.
 Typically the <errno.h> provided with the C library may be used, if such a
 library is used. Otherwise, an <errno.h> similar file must be provided.
 
 Notice that the errno.h include file must also provide an extern definition or
 macro to access the 'errno' variable. The macro may handle 'errno' globally,
 per thread, per process or whatever. In a multi-threaded environment, the
 'errno' variable should be defined per thread, otherwise using 'errno' may
 not be reliable.

 The error codes used by this SW are:

 EPERM         Operation not permitted
 ENOENT        No such file or directory
 EIO           I/O error
 ENXIO         No such device or address
 ENOMEM        Not enough memory
 EACCES        Permission denied
 EBUSY         Resource is busy
 EEXIST        File exists
 ENODEV        The wrong type of device was given to a function that expects a
               particular sort of device.
 ENOTDIR       Not a directory
 EISDIR        Is a directory
 EINVAL        Invalid argument
 ENOSPC        No space left on device
 ENOTEMPTY     Directory not empty
 ENAMETOOLONG  File or path name too long

 If the environment provides perror() and strerror() functions (outside the scope
 of this SW), then these functions may be used to show more detailed error
 info when SHRD_ERRNO is enabled.

 Since 'errno' is only updated in error situations, the run-time performance
 is not affected. It will however add some extra codelines in order to set
 'errno', although the contribution due to this is very small.
*******************************************************************************/

#define SHRD_ERRNO                    0     /* [0, 1, 2] */



/*******************************************************************************
          ONLY MODIFY IF USING ERRNO UPDATING AND REQUIRED TO CHANGE
 ******************************************************************************/

#if (SHRD_ERRNO)

/* Or replace with appropriate include file defining error numbers */
#include <errno.h>

/* Macro for setting errno. Check if debugging should be showed */
#if (SHRD_ERRNO == 2)

/* Dump and update. Notice that strerror() may not be reentrant, but */
/* this is just for debugging... */
#define SHRD_SET_ERRNO(x)  \
{ \
   shrdPrintf(("[%s:%d] %s\n", __FILE__, __LINE__, strerror(x))); \
   errno = x; \
}

#else

/* Just update errno */
#define SHRD_SET_ERRNO(x)  errno = x

#endif

#else

#define SHRD_SET_ERRNO(x)

#endif



/*******************************************************************************
                             C GLUE FUNCTIONS
 *******************************************************************************
 Miscellaneous glue functions that follows the C standard library. If functions
 are not available through C libraries, they must be mapped into similarly related
 functions in the application.

 NOTICE: If the below functions are defined to C library functions, appropriate
         standard C include files may need to be added to the top of this file.
         This depends on the environment.

 All the below glue functions MUST be reentrant if running in a preemptive
 environment!
*******************************************************************************/

/* Prototype: void free(void *memblock); */
#define shrdFree                      free

/* Prototype: void *malloc(size_t size); */
/* Requirement: Allocated memory must start at a 32 bit aligned address */
#define shrdMalloc                    malloc

/* Prototype: int memcmp(const void *buf1, const void *buf2, size_t count); */
#define shrdMemcmp                    memcmp

/* Prototype: void *memcpy(void *dest, const void *src, size_t count); */
#define shrdMemcpy                    memcpy

/* Prototype: void *memset(void *dest, int c, size_t count); */
#define shrdMemset                    memset

/* Prototype: int printf(const char *format [, argument]...); */
#define shrdPrintf(x)                 do {} while(0)

/* Prototype: int strcmp(const char *string1, const char *string2); */
#define shrdStrcmp                    strcmp

/* Prototype: char *strcpy(char *strDestination, const char *strSource); */
#define shrdStrcpy                    strcpy

/* Prototype: size_t strlen(const char *string); */
#define shrdStrlen                    strlen


#endif /* SHRDCONF_H */
