
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             show.c
//      General "Show Information" functions.
//
//=============================================================================

#include "string.h"
#include "stdlib.h"

#include "hw.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "fixdata.h"

#include "struct.h"
#include "appdata.h"
#include "defines.h"
#include "msg.h"
#include "screen.h"
#include "show.h"
#include "util.h"
#include "utllib.h"

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
//	ShowSWVersion	Display Software Version.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

extern void ShowSWVersion( void )
{
	// Display the software version 
	memcpy( CSTMSG, VERSION, 10 );
	CSTMSG[10] = '\0';

	// Display the downline load revision 
	BufAsc( ( char * ) CSTMSG2, &TCONF.TCDLL );
	CSTMSG2[2] = '\0';

    // Display the BOOTER version 
    // "BOOTER FU065A20" string expected from EFTOS
    // skip "BOOTER" word
    const char *ptr;
    for ( ptr = HW_Booter_Version(); *ptr != ' '; ptr++ );

    // Copy the rest
    strcpy (CSTMSG3, ptr);

	// Display the terminal ID 
	memcpy( CSTMSG4, (UBYTE *)TERM.TERMSer, 8 );
	CSTMSG4[8] = '\0';

	// See if 8 digit Serial Number required 
	if ( TERM.TERMOpt1 & TM1_TSER8 )
	{
		// Yes; 8 digit Serial Number for display 
		CSTMSG4[8] = '\0';
	}
	else
	{
		// No, 9 digit Serial Number 

		// Add Terminal Serial Number Check Digit to display 
		CSTMSG4[8] = TERM.TERMChk;
		CSTMSG4[9] = '\0';
	}

	// Display Screen 
	DrawScreen( SOFT_VER_SCR );

}


//=============================================================================
// Private function definitions
//=============================================================================

