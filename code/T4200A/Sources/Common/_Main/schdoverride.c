
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             schdoverride.c
//      Schedule Functions
//
//=============================================================================
// System includes
#include "string.h"

// HSDK includes
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "utllib.h"
#include "fixdata.h"

// Application includes
#include "transdef.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "tables.h"

// This module external declaration
#include "schdoverride.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================

#define  SCHIDLEMINS	0x10	// Default 10 minutes of required idle time
								// prior to starting a scheduled event 


//=============================================================================
// Private function declarations
//=============================================================================

static Bool SchdIdleTime( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static struct schedule_rec *pSchPtr;	// Pointer to SCHTAB entry      


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     When any input is received from the user in the idle status.
//!     If reads each schedule table entry and verifies that current time is
//!     within scheduled idle minutes to scheduled time. The event is cleared
//!     from the schedule table, because the terminal is too busy to perform
//!     the scheduled event.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void SchdOverride( void )
{
	// See if required schedule idle time is not null 
	if ( 0x00 != SCHIDLEMINS )
	{
		// Search schedule table for the entry used 
		for ( pSchPtr = SCHTAB; pSchPtr < &SCHTAB[SCHMAX]; pSchPtr++ )
		{
			// Look at every entry to see if this entry is used. 
			if ( pSchPtr->SCHFLAG & SCH_USED )
			{
				// See if current time is within terminal configure idle 
				// minutes to scheduled time 
				if ( SchdIdleTime(  ) )
				{
					// Yes, Terminal is too busy, Clear bit 0 of 
					// this entry's flag 
					pSchPtr->SCHFLAG = 0;

					// Recalculate the LRC 
					SetSCHTabLRC(  );
				}
			}
		}
	}

	return;
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Check schedule table entry to see if current time is within scheduled
//!     idle minutes to scheduled time.
//!
//! \return
//!     True if Terminal is too busy to perform any scheduled Event else False
//-----------------------------------------------------------------------------
static Bool SchdIdleTime( void )
{
	UBYTE retval;
	UBYTE RtcBuf[RTCLEN];		// RTC Read Buffer            
	UBYTE RtcBcd[5];			// Current YYMMDDHHMM in BCD.  
	UBYTE SchBcd[5];			// Copy of scheduled time         

	// Default return value to False 
	retval = False;

	// Read the real time clock.  
	SDK_RtcRead( RtcBuf );

	// Convert current YYMMDDHHMM to BCD 
	AscHex( RtcBcd, ( char * ) &RtcBuf[0], ( UBYTE ) ( S_SMWHEN - 1 ) );

	// Get the scheduled time from schedule event 
	memcpy( SchBcd, pSchPtr->SMWHEN, ( UWORD ) ( S_SMWHEN - 1 ) );

	// See if today's date is the same as scheduled date 
	if ( SchBcd[2] == RtcBcd[2] )
	{
		// Yes, See if Current time is less than scheduled time 
		if (   ( RtcBcd[3] < SchBcd[3] ) 
			|| (   ( RtcBcd[3] == SchBcd[3] ) 
				&& ( RtcBcd[4] < SchBcd[4] ) ) )
		{
			// Yes, Add scheduled idle minutes to current time 
			RtcBcd[4] += SCHIDLEMINS;

			// Adjust current hour and minutes 
			if ( RtcBcd[4] >= 0x60 )
			{
				// Increase an hour 
				RtcBcd[3] += 0x01;

				// Get the minutes 
				RtcBcd[4] -= 0x60;
			}

			// See if Adjusted current time is greater than or equal to scheduled time 
			if (   ( RtcBcd[3] > SchBcd[3] ) 
				|| (   ( RtcBcd[3] == SchBcd[3] ) 
					&& ( RtcBcd[4] >= SchBcd[4] ) ) )
			{
				// Current time is within scheduled idle minutes to scheduled time 
				retval = True;
			}
		}
	}

	return retval;
}
