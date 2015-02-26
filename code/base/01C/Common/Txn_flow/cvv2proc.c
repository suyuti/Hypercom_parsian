//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             cvv2proc.c
//      CVV2 processing routines
//
//=============================================================================

#include "string.h"
#include "stdlib.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "tables.h"

#include "cvv2proc.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static Bool InCVV2Ind( void );
static Bool InCVV2( void );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
// Setup screen entry prompt structures

static const ENTRY_STR CVV2Entry =	// Prompt for CVV2 Value
{
	N_NullStr,					// Prompt
	N_CVV2Entry,				// Default
	N_NullStr,					// Default
};


//=============================================================================
// Public function definitions
//=============================================================================


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Process CVV2 input.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern UBYTE CVV2Proc( void )
{

	// Initialize return value
	UBYTE chRetval = False;

	// Prompt to get CVV2 Presence Indicator
	if ( InCVV2Ind(  ) )
	{
		// Prompt to get CVV2 Value
		if ( InCVV2(  ) )
			chRetval = True;
	}
	return chRetval;
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Get CVV2 Presence Indicator
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  => if successful;
//!                 False => if timed out or user cancelled.
//!
//-----------------------------------------------------------------------------
static Bool InCVV2Ind( void )
{
	UBYTE PresInd;

	// See if PAN manually entry and CVV2 turned on
	if ( ( TRINP.TRPOSE[1] & BIT_4_SET ) &&
		 ( (TRINP.TRISPTR->ISOPT5) & IS5_CVV2 ) )
	{

		// Draw the CVV2 Indicator Screen
		DrawScreen( CVV2_IND_SCR );

		while ( True )
		{
			// Get the CVV2 Presence Indicator from the User
			PresInd = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

			// Check the validity of the input
			switch ( PresInd )
			{
				case CLEAR_KY:	// CLEAR key pressed; return False
				case CANCEL_KY:	// CANCEL key pressed
				case 0:		// Timed out
					return False;

				case '0':
				case '1':
				case '2':
				case '9':
					// Move CVV2 Indicator into TRINP structure.
					TRINP.TRCVV2IND = PresInd;

					return True;

				default:
					// Invalid entry; notify user
					SDK_Beeper( TENMS * 40 );

					break;
			}
		}
	}

	return True;
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Get CVV2 Value
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  => if successful;
//!                 False => if timed out or user cancelled.
//!
//-----------------------------------------------------------------------------
static Bool InCVV2( void )
{
	UBYTE length;

	// Set cvv2 value to spaces
	memset( ( char * ) TRINP.TRCVV2, ' ',S_TRCVV2 );

	// See if PAN manually entry and CVV2 turned on
	if ( ( TRINP.TRPOSE[1] & BIT_4_SET ) &&
		 ( (TRINP.TRISPTR->ISOPT5) & IS5_CVV2 ) )
	{
		// See if CVV2 value must be present
		if ( '1' == TRINP.TRCVV2IND )
		{
			if ( !AlphaNumEntry
				 ( 1, S_TRCVV2, 1, ( ENTRY_STR * ) & CVV2Entry ) )
			{
				// User cancelled or timed out
				return False;
			}

			memset( ( char * ) TRINP.TRCVV2, ' ', S_TRCVV2 );

			length = StrLn( Dspbuf, sizeof( Dspbuf ) );

			// Move CVV2 on the format " XXX" into TRINP structure.
			memcpy( &TRINP.TRCVV2[S_TRCVV2 - length], Dspbuf, length );
		}
	}

	return True;
}
