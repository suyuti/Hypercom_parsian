
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             getrninv.c
//      Invoice number processing
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "util2.h"
#include "journal.h"

#include "getrninv.h"

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
static const ENTRY_STR GetInvEntry = {
	EnterInvNum,				// Title
	N_NullStr,					// Prompt
	N_NullStr,					// Default value
};


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//!  \brief
//!     Get transaction invoice number.
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True - OK
//!                 False - cancelled
//-----------------------------------------------------------------------------
extern Bool GetTranInv( void )
{
    static UWORD JrnIdent;
    UBYTE Retval;

	// Lock the journal first
	JournalLock(  );

	// Set up selection data in common record JSREC
	SetDefaultSelection(  );

	// Don't retreive any reversals or voids or settled
	JSREC.JSSTATOFF = TS1_VOID + TS1_REV + TS1_SETL;

	do
	{
		GlobalDsp = 0;

		// Set title for AlphaNumEntry()
		Retval = AlphaNumEntry( 1, S_TRINV, 1, &GetInvEntry );

		if ( 0 == Retval )
		{
			// Cancel key pressed.

			// Release the lock on the journal semaphore.
			JournalRelease(  );

			return False;
		}

		// Copy invoice number that was input to search record
		MovAscZFill( ( char * ) JSREC.JSINV, Dspbuf, S_TRINV, ' ' );

		// JSREC will be used by JournalSelFirst()
		// JrnIdent will be set with first ident, if one exists.
		JrnIdent = JournalSelFirst(  );

		if ( 0x0000 == JrnIdent )
		{
			// No entries found in the journal, void or otherwise
			ShowErrMsg( InvNumberMissing );
		}
	}
	while ( 0x0000 == JrnIdent );

	// Read the record that was found
	JournalRead( JrnIdent, &TRINP );

	//Save journal number	 @AAMELIN
	TRINP.TRJRNIDENT = JrnIdent;

	// Release the lock on the journal semaphore.
	JournalRelease(  );

	// Return good status
	return True;
}
