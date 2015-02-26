

//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             shift.c
//      Change shift number.
//
//=============================================================================

#include "string.h"
#include "stdlib.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "util.h"
#include "tables.h"

#include "shift.h"

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
//	ChangeShift		Change the shift number.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			Shift 1 is the smallest valid number.
//
//-----------------------------------------------------------------------------

extern void ChangeShift( void )
{
	UBYTE choice;
	Bool Done;

	// Shift change is only valid if option is set from TermMaster. 
	if ( !( TCONF.TCOPT5 & TC5_SHIFT ) )
	{
		ShowErrMsg( N_FuncNotSupported );
		return;
	}

	DrawMenu( MENU_SHIFT );

	// Do until done 
	do
	{
		// Set done flag 
		Done = True;

		choice = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

		// Check number entered 
		switch ( choice )
		{
			case 'I':			// Increment 
				IncBcdLen( ( UBYTE * ) & TFTAB.TFSHIFT, 1 );
				SetTFTabLRC(  );
				break;

			case 'R':			// Reset 
				TFTAB.TFSHIFT = 1;
				SetTFTabLRC(  );
				break;

			case 0:				// Cancel or time-out 
			case CANCEL_KY:
			case CLEAR_KY:
			case B_PREVIOUS_NEW_MENU:
				break;

			default:			// Unknown input; notify user. 
				SDK_Beeper( TENMS * 40 );

				// Keep getting user input 
				Done = False;
				break;
		}
	}
	while ( !Done );
}


//=============================================================================
// Private function definitions
//=============================================================================
