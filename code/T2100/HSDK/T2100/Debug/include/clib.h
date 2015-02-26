/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef __CLIB_H
#define __CLIB_H

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupOSC
	@{
*/

//!	\brief Semaphore type declaration.
//!	
//!	Usually not used by the applications due to poor design.
//!
typedef unsigned char SEMAPHORE;

//!
//!	\brief Convert 2-digit BCD number to a binary byte value.
//!
//!	\param bcd BCD number to convert.
//!	\retval char    Binary equivalent of the BCD number.
//!
extern unsigned char bcdtob( unsigned char bcd );

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif	// _CLIB_H_
