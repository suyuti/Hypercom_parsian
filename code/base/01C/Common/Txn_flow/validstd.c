
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             validstd.c
//      Validate card start date
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

#include "validstd.h"

//!-----------------------------------------------------------------
//! \brief
//!     Validate the card expiration date
//!
//! \param
//!     None
//!
//! \return
//!     char        0 - Bad date or expired.
//!	    		    1 - Expiration date validated.
//!-----------------------------------------------------------------

char ValidateStartDate( void )
{
   static char   tmp_t[21];
   static int ThisYear, CardYear, TempYear;
   static int ThisMonth, CardMonth;
   static char retval;

	retval = 0;

	/* For manual entry and start date entry was disabled, we will skip
	   the validation and return 1. */
	if((INJECTKEY) && !((TRINP.TRISPTR->ISOPT6) & IS6_STARTD))
		return 1;

	if((TRINP.TRSTARTD[1] >= 1) && (TRINP.TRSTARTD[1] <= 0x12))
	{
    	SDK_RtcRead( (UBYTE *)tmp_t );

		/* calculate card start  date - year portion */
		CardYear = (int) ((TRINP.TRSTARTD[0]>>4) * 10 + (TRINP.TRSTARTD[0] & 0x0f));

		/* calculate current date - year portion */
		ThisYear = (int) ((tmp_t[0] - '0') * 10 + (tmp_t[1] - '0'));

		/* -50 is an arbitrary number to handle the year 2000 problem. Any card
		   that is 50 years old should not be floating in the market. */

		TempYear = ThisYear - CardYear;
		
		if(((TempYear > 0) && (TempYear < 50)) || (TempYear < -50))
			retval = 1;
		else
			if(0 == TempYear)
			{
				CardMonth = (int) ((TRINP.TRSTARTD[1]>>4) * 10 + (TRINP.TRSTARTD[1] & 0x0f));
				ThisMonth = (int) ((tmp_t[2] - '0') * 10 + (tmp_t[3] - '0'));
				if(	CardMonth <= ThisMonth)
					retval = 1;
			}

	}
	else
	{
		retval = 1;
	}

	return retval;
}
