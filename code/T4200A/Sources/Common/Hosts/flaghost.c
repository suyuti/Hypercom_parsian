//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             flaghost.c
//      Function to flag host process
//
//=============================================================================
#include "string.h"
#include "basictyp.h"
#include "fixdata.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkterm.h"
#include "sdktime.h"

#include "struct.h"
#include "util.h"
#include "appdata.h"
#include "defines.h"
#include "utllib.h"
#include "receipt.h"
#include "flaghost.h"
#include "comm.h"
#include "events.h"

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
//! \brief
//!     Flags the host process and wait for host completion
//!
//! \return
//!     None
//!
//! \note
//!     Allows cancel using CLEAR key if FLAG_ALLOW_CANCEL is set. Preprint the
//!     receipt if FLAG_PREPRINT is set.
//-----------------------------------------------------------------------------
extern void FlagHost( void )
{
	Bool bPinCardStatus;
	EVENT_ID ev;

	memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );
	memset( ( UBYTE * ) CSTMSG2, 0, sizeof( CSTMSG2 ) );
	GetMsg( N_Comms, CSTMSG4 );
	Enable_Flags[3] = 1;
	Enable_Flags[4] = 0;
	GetMsg( PleaseWait, CSTMSG5 );
	Enable_Flags[SHOWAD_FLG] = 0;
	DrawScreen( INFO_SCR );

	// Clear the PreDialAbort flag since we are passing to the host.
	PreDialAbort = False;

	/* Initialise 'Cancelled by user' flag */
	OnlineCancelled = False;

	// Flag the host process
	TRRSPREADY = 0;
	SDK_ClearEvents( EVENT_HOSTRSP );
	TRREQBLD |= BIT_0_SET;

	SDK_SetEvents( EVENT_HOSTQRY );

	if ( FlagHostCntl & FLAG_PREPRINT )
	{
		// Print receipt header if requested in flag byte.
		PreprintReceipt ();
	}

	// Wait for completion of Initialization
	while ( SDK_WaitForEvents( EVENT_HOSTRSP, &ev, ONESECOND / 4 ) )
	{
		// Allow the user to cancel according to flag byte setting.
		if ( FlagHostCntl & FLAG_ALLOW_CANCEL )
		{
			// Pause a second or so to look for a key press keys
			read_io( DEV_PAD | DEV_KBD | STAY_OPEN0, 1, &bPinCardStatus, &DevFlag );	// Wait for input

			// See if CLEAR key was pressed
			if ( ( CLEAR_KY == KeyVal ) || ( CANCEL_KY == KeyVal ) )
			{
				// Check again because this can be changed when read_io prosecced  
				if ( FlagHostCntl & FLAG_ALLOW_CANCEL )
				{
					// Set 'Cancelled by User' flag
					OnlineCancelled = True;

					// Yes; attempt to cancel dialing and hang up line
					CancelComms ();
				}
			}
		}
	}
	SDK_ClearEvents( EVENT_HOSTRSP );

	// Reset the FlagHostCntl flag on exit.
	FlagHostCntl = 0;
}

