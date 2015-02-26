
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             inputpwd.c
//      Input password routine
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "utllib.h"
#include "fixdata.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "util.h"

#include "inputpwd.h"

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
static ENTRY_STR PswdEntry;

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Save and Verify Input Password
//!
//!  \param
//!     msg_id      enum mid id of message to display on top line
//!
//!  \return
//!     Bool        True  - if password entered is okay
//!                 False - if password not entered or password
//!                         entered is bad.
//!
//!  \note
//!     Broke this one out of FUNCTION.C so it can be
//!     used by modules in all pages
//!
//-----------------------------------------------------------------------------
extern Bool InputPswd( enum msg_id mid )
{
	UBYTE Status;

	// NumEntry parameters
	PswdEntry.TitleMid = mid;
	PswdEntry.Line1Mid = N_NullStr;
	PswdEntry.Line2Mid = EnterPswd;

	while ( True )
	{
		memset( ( UBYTE * ) Dspbuf, 0,5 );	// Initialize input buffer
		// Prompt for Input Password (4 Digits)
		Status = NumEntry( 2, 4, 4, &PswdEntry );

		// Check if Input Password has been entered
		if ( 1 == Status )
		{
			// Yes; save the Input Password entered
			memcpy( TRINP.TRPWD, Dspbuf, 4 );

			// Verify Input Password entered
			if ( memcmp( TRINP.TRPWD, (UBYTE *)TERM.TERMPwd, 4 ) == 0 )
			{
				// Correct; return True
				return ( True );
			}
			else
			{
				// Incorrect password, notify user
				SDK_Beeper( TENMS * 40 );
			}
		}
		else
		{
			// No; cancel, exit the loop
			break;
		}
	}

	// Return False
	return ( False );
}

//=============================================================================
// Private function definitions
//=============================================================================
