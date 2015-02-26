/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//!
//! \file basictyp.h 
//!     \brief Recommended set of standard typedefs and defines. 
//!     \since 1995
//!

#ifndef BASICTYP
#define BASICTYP

/*
=============================================================================
*       Recommended set of standard typedefs and defines.                   *
*       This file should be included in all 'C' source code files.          *
=============================================================================
*/

#if !defined(__GNUC__) && !defined(__attribute__)
#define __attribute__(A)
#endif

#if defined(MAKE_OPTIMUM)
#define section(A)
#endif


/*
 *      Define a basic Boolean type
 */
#ifdef __cplusplus
#define False 0
#define True 1
typedef unsigned char Bool;
#else
typedef enum { False = 0, True } __attribute__((packed)) Bool;
#endif

#ifdef _TARG_Z80
#include "types.h"
#else
#include "hyctypes.h"
#endif
/*
 *      Define pointers to functions of various types.
 *
 *      Uniware Default Size in bytes:  2
 *
 */
typedef unsigned int          ( *pFUNCint )( void );
typedef const pFUNCint        pcFUNCint;

typedef void                  ( *pFUNCvoid )( void );
typedef const pFUNCvoid       pcFUNCvoid;


/*
 *      PRIVATE is defined to show the intent to hide data, rather than
 *      make it persistent beyond a function's scope.
 */
#define PRIVATE static


#endif  /* ifndef BASICTYP */

