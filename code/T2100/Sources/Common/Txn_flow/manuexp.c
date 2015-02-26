
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             ManualExp
//      Routine for maunal entry of expiration date
//
//=============================================================================

#include "basictyp.h"

#include "struct.h"
#include "appdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"

#include "manuexp.h"

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
static const ENTRY_STR ExpDateEntry = {
	N_NullStr,					// Default Title
	ExpireDate,					// Prompt
	N_NullStr,					// Default value
};


//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  ManualExp       Accept expiration date from keyboard
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          Data will be stored in global buffer TRINP.TREXPD in BCD.
//                  Global variable chRetval contains the status of the entry:
//                  0 CANCEL_KY key was pressed
//                  1 data was entered successfully
//
//-----------------------------------------------------------------------------

extern UBYTE ManualExp( void )
{
	UBYTE chRetval;

	chRetval = NumEntry( 1, 4, 4, &ExpDateEntry );

	if ( chRetval )
	{
		// Save the EXP date, Reverse the order because it was entered
		// as MMYY. To match that from a Card Swipe, we need to swap them

		TRINP.TREXPD[0] = Dspbuf[2] - '0';
		TRINP.TREXPD[0] <<= 4;
		TRINP.TREXPD[0] |= ( Dspbuf[3] - '0' );
		TRINP.TREXPD[1] = Dspbuf[0] - '0';
		TRINP.TREXPD[1] <<= 4;
		TRINP.TREXPD[1] |= ( Dspbuf[1] - '0' );
	}

	return chRetval;
}

//=============================================================================
// Private function definitions
//=============================================================================
