
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             cstmamnt.c
//      Builds into the global custom message buffer
//      CSTMSG, a Title and Amount Routine
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "cvtamt.h"
#include "negsign.h"

#include "cstmamnt.h"

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


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Builds into the global custom message buffer, CSTMSG, a Title and Amount
//!
//!  \param
//!     Mid         message id
//!  \param
//!     pAmount     pointer to the Amount.
//!
//!  \return
//!       None
//!
//-----------------------------------------------------------------------------
extern void CustomAmount( enum msg_id Mid, UBYTE * pAmount )
{
	char Title[21];
	UBYTE Length;
	UBYTE i;

	// Get the wanted display message title
	GetMsg( Mid, Title );

	// Blank out the custom message buffer
	memset( CSTMSG, ' ',sizeof( CSTMSG ) );

	// Null terminate the Custom Message
	CSTMSG[20] = '\0';

	// Convert to ASCII Amount
	CvtAmt( &CSTMSG[7], pAmount );

	// See if Refund
	if ( CurrentEvent.TRNOPT1 & T1_CREDIT )
	{
		// Yes; add a '-' sign to the display
		NegSign( &CSTMSG[7], 13 );
	}

	// Get the length of the Title
	Length = StrLn( Title, sizeof( Title ) );

	// Add the Title; only overwrite spaces
	for ( i = 0; i < Length; i++ )
	{
		// See if space
		if ( ' ' == CSTMSG[i] )
		{
			// Yes; move the character
			CSTMSG[i] = Title[i];
		}
		else
		{
			// No; exit the loop
			break;
		}
	}
}
