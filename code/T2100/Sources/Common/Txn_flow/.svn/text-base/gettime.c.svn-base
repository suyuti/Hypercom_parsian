
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             gettime.c
//      Time function
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "sdkmem.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "util.h"
#include "gettime.h"


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
//  GetTime         Get a time entry from the keyboard
//
//  Parameters:     const ENTRY_STR * es    Pointer to entry screen structure
//
//  Global Inputs:
//
//  Returns:        int     0 if user cancelled or timed out
//                          1 if valid time entered (in Dspbuf HHMM YYMMDD)
//                          2 if no time entered
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern int GetTime( const ENTRY_STR * es )
{
	char dispDT[15];
	char hour, min;

	// Loop forever (or until break or return)
	while ( True )
	{
		// Get the current date and time
		SDK_RtcRead( ( UBYTE * ) dispDT );

		// Format a string like "10:30"
		// Hour
		CSTMSG[0] = dispDT[6];
		CSTMSG[1] = dispDT[7];
		CSTMSG[2] = ':';
		// Minute
		CSTMSG[3] = dispDT[8];
		CSTMSG[4] = dispDT[9];
		CSTMSG[5] = 0;

		// Prompt for time
		if ( !NumEntry( 1, 4, 0, es ) )
			break;				// User cancelled or timed out

		// Check for ENTER Key only
		switch ( StrLn( Dspbuf, sizeof( Dspbuf ) ) )
		{
			case 0:			// Just ENTER pressed
				// Return the current time & date
				memcpy( &Dspbuf[0], &dispDT[6], 4 );	// HHMM
				Dspbuf[4] = ' ';
				memcpy( &Dspbuf[5], &dispDT[0], 6 );	// YYMMDD
				Dspbuf[11] = 0;

				return 2;		// No time entered

			case 3:			// HMM entered
				// Move over one to form HHMM
				Dspbuf[3] = Dspbuf[2];
				Dspbuf[2] = Dspbuf[1];
				Dspbuf[1] = Dspbuf[0];
				Dspbuf[0] = '0';
				// fall through to case 4:

			case 4:			// HHMM entered
				// Convert hour and minutes to binary
				hour = ( Dspbuf[0] - '0' ) * 10 + ( Dspbuf[1] - '0' );
				min = ( Dspbuf[2] - '0' ) * 10 + ( Dspbuf[3] - '0' );

				// Concatenate current date (for Fun_10 to write real time clock)
				Dspbuf[4] = ' ';
				memcpy( &Dspbuf[5], &dispDT[0], 6 );	// YYMMDD
				Dspbuf[11] = 0;

				// Check if valid hour and minutes entered
				if ( ( hour <= 23 ) && ( min <= 59 ) )
				{
					return 1;	// Valid time entered
				}
		}

		// This is not a valid entry
		ShowErrMsg( InvalidEntry );
	}

	return 0;					// User cancelled or timed out
}

//=============================================================================
// Private function definitions
//=============================================================================
