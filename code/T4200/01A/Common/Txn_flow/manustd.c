
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     manustd.c
//    Function to entry card start date
//
//=============================================================================

#include "basictyp.h"

#include "struct.h"
#include "appdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"

#include "manustd.h"

static const ENTRY_STR StartDateEntry =
{
	N_NullStr,			/* Default Title */
	StartDate,				/* Prompt */
	N_NullStr,				/* Default value */
};

//!-----------------------------------------------------------------------
//! \brief     Accept expiration date from keyboard
//!
//! \param
//!     None
//!
//! \return
//!     None
//!
//! \note
//!     Data will be stored in global buffer TRINP.TREXPD in BCD.
//!		Global variable chRetval contains the status of the entry:
//!  	0 CANCEL_KY key was pressed
//!		1 data was entered successfully
//!
//!-----------------------------------------------------------------------
UBYTE ManualStartDate(void)
{
	UBYTE chRetval;

	chRetval = NumEntry( 1, 4, 4, &StartDateEntry );

	if(chRetval)
	{
		/* Save the START date, Reverse the order because it was entered
		   as MMYY. To match that from a Card Swipe, we need to swap them */

		TRINP.TRSTARTD[0] = Dspbuf[2]- '0';
		TRINP.TRSTARTD[0] <<= 4;
		TRINP.TRSTARTD[0] |=  (Dspbuf[3] - '0');
		TRINP.TRSTARTD[1] =  Dspbuf[0] - '0';
		TRINP.TRSTARTD[1] <<= 4;
		TRINP.TRSTARTD[1] |=  (Dspbuf[1] - '0');
	}

	return chRetval;
}
