
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             aptab.c
//      Additional Prompts table functionality
//
//=============================================================================

#include "clib.h"

#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "comdata.h"

#include "aptab.h"


//-----------------------------------------------------------------------------
//  SetAPTabLRC     Set the Checksum (LRC) for the Additional prompt table
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          Upon booting up, if checksum is wrong,
//                  application will report error and will not run.
//-----------------------------------------------------------------------------

extern void SetAPTabLRC( void )
{
	APLRC = CalcLrc( ( UBYTE * ) & APTAB, APSZE * APMAXENT );
}

