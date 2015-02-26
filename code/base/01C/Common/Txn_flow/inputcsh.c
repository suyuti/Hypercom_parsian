
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             inputcsh.c
//      Input cashier number routine
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "util.h"
#include "cashier.h"

#include "inputcsh.h"

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
//!  \brief
//!     Set up to cross call input cashier function.
//!
//!  \param
//!     msg_id      enum Mid Message Id
//!  \param
//!     AllowNull   Allow null flag.
//!
//!  \return
//!     Bool        True  - successful,
//!                 False - otherwise.
//!
//-----------------------------------------------------------------------------
extern Bool InputCashier( enum msg_id Mid, Bool AllowNull )
{
	Bool Status;
	BYTE Index;

	// See if Cashier Feature is on
	if ( !( TCONF.TCOPT1 & TC1_CASHIER ) )
	{
		// No; return okay
		return True;
	}

	// See if Adjust or Close Tab Transaction
	if ( ( ADJUST == TRINP.TRKEY ) || ( CLOSETAB == TRINP.TRKEY ) )
	{
		// Yes; Adjust only Cashier entered?
		if ( !( TCONF.TCOPT4 & TC4_SRV_ADJ ) )
		{
			// No; return okay
			return True;
		}
	}
	else
	{
		// No; see if Batch Review
		if ( REVIEW == TRINP.TRKEY )
		{
			// Yes; Review only Cashier entered?
			if ( !( TCONF.TCOPT4 & TC4_SRV_BAT ) )
			{
				// No; return okay
				return True;
			}
		}
	}

	// Reset the status
	Status = False;

	// Loop while invalid Cashier Number entered
	do
	{
		// Get Cashier Number from user
		Index = GetCashier( AllowNull );

		// See if valid Cashier Number entered or not needed
		if ( Index >= -1 )
		{
			// See if valid Cashier Number entered
			if ( Index >= 0 )
			{
				// Yes; move the Cashier Number
				memcpy( TRINP.TRCASHIER, Dspbuf, S_TRCASHIER );

				// Move the Till Number
				memcpy( TRINP.TRTILLNO, CASHTAB[Index].TILLNO, S_TRTILLNO );
			}

			// Set status to okay
			Status = True;
		}
	}
	while ( -3 == Index );

	// Return the status
	return Status;
}

//=============================================================================
// Private function definitions
//=============================================================================
