

//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             appdwcc.c
//      Appends Write Control Character to send data

//=============================================================================
//=============================================================================
#include "basictyp.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "hostutil.h"


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
//  AppendWrtCtrlChar       Appends Write Control Character to send data
//
//  Parameters:             None.
//
//  Global Inputs:          None.
//
//  Returns:                Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendWrtCtrlChar( void )
{
	// See if a value exists in the 'TRTRK2' field
	if ( TRREQ.TRTRK2[0] )
	{
		// Yes; then information is from Cardreader

		// Add Track 2 read indicator flag to the send data
		AppendChar( 'A' );
	}
	else
	{
		// No; add Pan was manually entered indicator flag to the send data
		AppendChar( '@' );
	}
}

//=============================================================================
// Private function definitions
//=============================================================================