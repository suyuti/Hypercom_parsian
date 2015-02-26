/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef __PTCS_H
#define __PTCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "execdef.inc"

/**
	@addtogroup GroupIOCS
	@{
*/

/**
	@brief Access to process timer control system.

	This function is the link from the application to the PTCS in the
	terminal operating system. All timer requests made through the SDK
	functions invoke this function.

	@param pTMBlk Pointer to timer request block.
*/
UBYTE ptcs( PTCS_BLK *pTMBlk );

/**
	@brief Initialize PTCS emulation layer.

	This function is need to be called before any use of PTCS emulation
	layer.

	@note HW_Start() should also be called before use of PTCS (may be
	      called before or after this function).
*/
void InitTimers( void );

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif  // _IOCS_H_
