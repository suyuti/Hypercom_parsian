/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

// Prototype for those C functions used from C++ code

#ifndef _SIM_INIT_
#define _SIM_INIT_

#include "execdef.inc"
#include "sysstruc.inc"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	ICE4000,
	ICE5500,
	ICE6000,
	ICE6500,
	T7PLUS,
	VIKING,
	OPTIMUM
} PLATFORM;

extern TERMREC EpromDefaults;

UBYTE SIM_CalcLRC( const UBYTE *pBuffer, UWORD BufferSize );

void SIM_SetPlatform( PLATFORM platform );
PLATFORM SIM_GetPlatform( void );

void SIM_BootInit( void );

#ifdef __cplusplus
}
#endif


#endif // _SIM_INIT_
