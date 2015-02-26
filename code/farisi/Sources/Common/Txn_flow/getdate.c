
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             getdate.c
//      Routine to get date
//
//=============================================================================

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
#include "getdate.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static Bool validDate( void );

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
//!     Get a date entry from the keyboard
//!
//!  \param
//!     es          Pointer to entry screen structure
//!  \param
//!     yearRequired    boolean to say if year entry is required
//!
//!  \return
//!     int         0 if user cancelled or timed out
//!                 1 if valid date entered (in Dspbuf, MMDDYY)
//!                 2 if no date entered
//!
//!  \note
//!     Alters CSTMSG
//!
//-----------------------------------------------------------------------------
extern int GetDate( const ENTRY_STR * es, Bool yearRequired )
{
	int minLength;
	char dispDT[15];

	if ( yearRequired )
		minLength = 5;
	else
		minLength = 0;

	// Loop forever (or until break or return)
	while ( True )
	{
		// Get the current date and time
		SDK_RtcRead( ( UBYTE * ) dispDT );

		// Format a string like "07/28/98"
		// Month
		CSTMSG[0] = dispDT[2];
		CSTMSG[1] = dispDT[3];
		CSTMSG[2] = '/';
		// Day
		CSTMSG[3] = dispDT[4];
		CSTMSG[4] = dispDT[5];
		CSTMSG[5] = '/';
		// Year
		CSTMSG[6] = dispDT[0];
		CSTMSG[7] = dispDT[1];
		CSTMSG[8] = 0;

		// Prompt for DATE
		if ( !NumEntry( 1, 6, ( char ) minLength, es ) )
			break;				// User cancelled or timed out

		switch ( StrLn( Dspbuf, 6 ) )
		{
			case 0:			// ENTER key only?
				// Return the current date
				// YY
				Dspbuf[4] = dispDT[0];
				Dspbuf[5] = dispDT[1];
				Dspbuf[6] = 0;
				return 2;		// No date entered

			case 3:			// MDD entered
				// Move over one to form MMDD
				Dspbuf[3] = Dspbuf[2];
				Dspbuf[2] = Dspbuf[1];
				Dspbuf[1] = Dspbuf[0];
				Dspbuf[0] = '0';
				// fall through to case 4:

			case 4:			// MMDD entered
				// Add in current year to form MMDDYY
				Dspbuf[4] = dispDT[0];
				Dspbuf[5] = dispDT[1];
				// fall through to case 6:

			case 6:			// MMDDYY Entered
				// Make sure Dspbuf is Null terminated
				Dspbuf[6] = 0;

				if ( validDate(  ) )
					return 1;

				break;

			case 5:			// MDDYY Entered
				// Make sure Dspbuf is Null terminated
				Dspbuf[6] = 0;

				// Move MDDYY one position to the right
				Dspbuf[5] = Dspbuf[4];
				Dspbuf[4] = Dspbuf[3];
				Dspbuf[3] = Dspbuf[2];
				Dspbuf[2] = Dspbuf[1];
				Dspbuf[1] = Dspbuf[0];

				// Add a leading zero
				Dspbuf[0] = '0';

				if ( validDate(  ) )
					return 1;

				break;
		}

		// This is not a valid entry
		ShowErrMsg( InvalidEntry );
	}

	// User cancelled or timed out
	return 0;
}

//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//!  \brief
//!     Validate the date in Dspbuf
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True if valid date entered (in Dspbuf, MMDDYY)
//!                 False otherwise
//-----------------------------------------------------------------------------
static Bool validDate( void )
{
	char month, day, year;
	static const char monthDate[12] =
		{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	// Convert month, day, & year from ASCII to binary
	month = ( Dspbuf[0] - '0' ) * 10 + ( Dspbuf[1] - '0' );
	day = ( Dspbuf[2] - '0' ) * 10 + ( Dspbuf[3] - '0' );
	year = ( Dspbuf[4] - '0' ) * 10 + ( Dspbuf[5] - '0' );

	// Are day and month valid values?
	// If Feb. 29, make sure it's a leap year
	if ( ( month >= 1 ) && ( month <= 12 ) &&	// Month
		 ( day >= 1 ) && ( day <= monthDate[( month - 1 )] ) &&	// Day
		 !( ( 2 == month ) && ( 29 == day ) & ( year % 4 != 0 ) ) )	// Leap year?
	{
		return True;			// Valid date entered
	}
	else
	{
		return False;
	}
}
