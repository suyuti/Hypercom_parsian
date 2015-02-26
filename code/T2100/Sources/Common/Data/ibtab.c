
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             ibtab.c
//      Idle screen buttons functionality
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "comdata.h"
#include "tabutil.h"

#include "ibtab.h"

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


//=============================================================================
// Private function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  SetIBTabLRC     Set the Checksum (LRC) for the ICE Idle Screen
//                  Button Table
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

extern void SetIBTabLRC( void )
{
	IBLRC = CalcLrc( ( UBYTE * ) & IBTAB, IBSZE );
}
//-----------------------------------------------------------------------------
//  Val_IBtab        Validate ICE buttons table content
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------
Bool Val_IBtab (void)
{
    // Check if table not filled
    if ((0 == IBTAB.IBBTN1LABEL1[0]) || (0 == IBTAB.IBBTN2LABEL1[0]) ||
        (0 == IBTAB.IBBTN3LABEL1[0]) || (0 == IBTAB.IBBTN4LABEL1[0]) )
    {
        // Yes, wrong table, Initialization required, no "Idle screen" allowed
        // "Init screen" takes a place
        IBFMT=0;
    }

    // Validate table
	return ValidateTable( &IBFMT, ( UBYTE * ) & IBTAB, IBSZE, &IBLRC );
}
