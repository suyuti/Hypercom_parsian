/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef _TYPES_H_
#define _TYPES_H_

#if defined(__cplusplus) && defined(MAKE_VIKING)
extern "C" {
#elif defined(MAKE_OPTIMUM)
#include "Plus/Nucleus.h"
#endif

/**
	@addtogroup GroupIOCS
	@{
*/

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#if (!defined(SSWDEF_H) && defined(MAKE_VIKING)) || (defined(MAKE_OPTIMUM) && !defined(_WINDEF_) && !defined(_HYP_COMPILER_SPECIFICS_))
/**
	@brief One-byte signed integer types.

	@note EFTOS defines this type as unsigned.
*/
typedef signed char BYTE;
/**
	@brief Two-byte signed integer type.

	@note EFTOS defines this type as unsigned.
*/
typedef signed short WORD;

#endif

/**
	@brief One-byte unsigned integer types.
*/
typedef unsigned char UBYTE;

/**
	@brief Two-byte unsigned integer type.
*/
typedef unsigned short UWORD;

/**
	@brief Four-byte unsigned integer type.
*/
typedef unsigned long UDWORD;

/**
	@brief Eight-byte signed integer type.
*/
typedef long long int __int64;

/**
	@brief An IP Address type is four bytes.
*/
typedef	UDWORD	IPADDRESS;

/**
	@brief OSRETURNCODES are defined in execdef.inc starting with OK and ERRADDR.

	Functions that return OS error code values should be declared as this
	type.
*/
typedef	UBYTE	OSRETURNCODES;

/**
	@brief Boolean true.

	@note In C code, boolean types should always be tested for zero or
	non-zero i.e. do not use "if (flag == TRUE)", instead use "if (flag)".
	The specific value of non-zero is not important.
*/
#if defined(MAKE_OPTIMUM) && !defined(_WINDEF_) && !defined(_HYP_COMPILER_SPECIFICS_)
#define	TRUE	-1
#elif defined(MAKE_VIKING)
#define	TRUE	1
#endif
/**
	@brief Boolean false.
*/
#if defined(MAKE_VIKING) || (defined(MAKE_OPTIMUM) && !defined(_HYP_COMPILER_SPECIFICS_))
#define	FALSE	0
#endif
/**
	@}
*/

#if defined(__cplusplus) && defined(MAKE_VIKING)
}
#endif

#endif	// _TYPES_H_
