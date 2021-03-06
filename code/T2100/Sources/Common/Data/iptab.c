
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             iptab.c
//        IcePak configuration functionality
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "comdata.h"

#include "iptab.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  SetIPTabLRC     Set the Checksum (LRC) for the ICEPAK 
//                  Configuration Table
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void SetIPTabLRC( void )
{
	IPLRC = CalcLrc( ( UBYTE * ) & IPTAB, IPSZE );
}

//=============================================================================
// Private function definitions
//=============================================================================
