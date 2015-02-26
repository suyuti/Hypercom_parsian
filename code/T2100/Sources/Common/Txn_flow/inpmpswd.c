
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             inpmpswd.c
//      Input maintenance password routine 
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "util.h"

#include "inpmpswd.h"

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
static const ENTRY_STR MPswdEntry = { 
	N_NullStr,					// Title
	MaintPswd,					// Prompt
	N_NullStr,					// Default value
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  EnterMPswd      Enter Maintenance Password if local term option
//                  is set
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        UBYTE       0 - if cancel key hit
//                              1 - if password entered 
//                              2 - no password (password is disabled).
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern UBYTE EnterMPswd( void )
{
	UBYTE Status;
	UBYTE Password[3];

	// Check if password is enabled (starts with "MP=")
	if ( memcmp( TCONF.TCMPWD, ( UBYTE * ) "MP=", 3 ) )
	{
		// No; Password not enabled, so ok
		return ( 2 );
	}

	// Check if password is disabled (all zeroes)
	if ( NullComp( ( char * ) &TCONF.TCMPWD[3], 3 ) )
	{
		// Yes; Password all zeroes, so ok
		return ( 2 );
	}

	// Display Maintenance Password Title

	// Loop till valid password entered or cancel
	while ( True )
	{
		// Prompt for Maintenance Password (6 Digits)
		Status = NumEntry( 2, 6, 6, &MPswdEntry );

		// Check if user cancel
		if ( Status == 0 )
		{
			// Yes; return False
			return ( 0 );
		}

		// Convert ASCII password to BCD
		AscHex( Password, Dspbuf, 3 );

		// Check if password is correct
		if ( memcmp( Password, &TCONF.TCMPWD[3], 3 ) == 0 )
		{
			// Password is correct, so break out of loop
			break;
		}
		else
		{
			// Password incorrect, notify user
			SDK_Beeper( TENMS * 40 );
		}
	}

	// Password has been verified
	return ( 1 );
}

//=============================================================================
// Private function definitions
//=============================================================================
