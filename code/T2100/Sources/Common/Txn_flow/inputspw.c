
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             inputspw.c
//      Input supervisor password routine 
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
#include "inputspw.h"

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
static ENTRY_STR SPswdEntry;


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  InputSPswd      Input Supervisor Password if Local Term Option
//                  is Set
//
//  Parameters:     enum msg_id mid     id of message to display on top line
//
//                  UBYTE PswdOption    TCLOCOP password option to check
//                                      for (0 - No Check).
//
//  Global Inputs:
//
//  Returns:        Bool    True  - if password entered is okay
//                          False - if password not entered or password
//                                  entered is bad.
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool InputSPswd( enum msg_id mid, UBYTE PswdOption )
{
	UBYTE Status;
	Bool PswdOkay;

	// Initialize password entered okay flag to False
	PswdOkay = False;

	// Password protect this option?
	if ( !( PswdOption & TCONF.TCLOCOP ) )
	{
		// NumEntry parameters
		SPswdEntry.TitleMid = mid;
		SPswdEntry.Line1Mid = N_NullStr;
		SPswdEntry.Line2Mid = EnterPswd;

		// Do until correct Password entered or cancel
		while ( True )
		{
			// Prompt for Input Password (4 Digits)
			Status = NumEntry( 2, 4, 4, &SPswdEntry );

			// Check if Input Password has been entered
			if ( 1 == Status )
			{
				// Verify Input Password entered
				if ( memcmp( (UBYTE *) Dspbuf, (UBYTE *)TERM.TERMPwd, 4 ) == 0 )
				{
					// Correct; set password entered okay flag to 
					// True
					PswdOkay = True;

					// Exit the loop
					break;
				}
				else
				{
					// Password incorrect, notify user
					SDK_Beeper( TENMS * 40 );
				}
			}
			else
			{
				// No; cancel, exit the loop
				break;
			}
		}
	}
	else
	{
		// No; set password entered okay flag to True
		PswdOkay = True;
	}

	// Return password entered okay flag
	return ( PswdOkay );
}


//=============================================================================
// Private function definitions
//=============================================================================
