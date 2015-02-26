
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             validexp.c
//      Validate the card expiration date.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "tables.h"

#include "validexp.h"

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
//!  \brief
//!     Validate the card expiration date.
//!
//!  \param
//!     None
//!
//!  \return
//!     char        0   Bad date or expired,
//!                 1   Expiration date validated.
//!
//-----------------------------------------------------------------------------
extern char ValidateEXP( void )
{
	static char tmp_t[21];
	static int ThisYear, CardYear, TempYear;
	static int ThisMonth, CardMonth;
	static char retval;

	retval = 0;

	// For manual entry and expiry date entry was disabled, we will skip
	// the validation and return 1. 
	if((INJECTKEY) && !((TRINP.TRISPTR->ISOPT1) & IS1_EXPD))
		return 1;

	if ( ( TRINP.TREXPD[1] >= 1 ) && ( TRINP.TREXPD[1] <= 0x12 ) )
	{
		SDK_RtcRead( ( UBYTE * ) tmp_t );

		// calculate card expiration  date - year portion 
		CardYear =
			( int ) ( ( TRINP.TREXPD[0] >> 4 ) * 10 +
					  ( TRINP.TREXPD[0] & 0x0f ) );

		// calculate current date - year portion 
		ThisYear = ( int ) ( ( tmp_t[0] - '0' ) * 10 + ( tmp_t[1] - '0' ) );

		// -50 is an arbitrary number to handle the year 2000 problem. Any card
		// that is 50 years old should not be floating in the market. 

		TempYear = CardYear - ThisYear;

		if ( ( ( TempYear > 0 ) && ( TempYear < 50 ) ) || ( TempYear < -50 ) )
		{
			retval = 1;
		}
		else if ( 0 == TempYear )
		{
			CardMonth =
				( int ) ( ( TRINP.TREXPD[1] >> 4 ) * 10 +
						  ( TRINP.TREXPD[1] & 0x0f ) );
			ThisMonth =
				( int ) ( ( tmp_t[2] - '0' ) * 10 + ( tmp_t[3] - '0' ) );
			if ( CardMonth >= ThisMonth )
				retval = 1;
		}
	}

	return retval;
}


//=============================================================================
// Private function definitions
//=============================================================================
