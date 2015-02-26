
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             inadjinv.c
//      Adjust/close tab routines
//
//=============================================================================

#include "string.h"
#include "stdlib.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "journal.h"
#include "msg.h"
#include "util.h"
#include "tables.h"
#include "chkbatch.h"

#include "inadjinv.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static Bool ChkAdjust( void );
static void SetAdjustJsrec( void );


//=============================================================================
// Public data definitions
//=============================================================================
Bool AdjAgain;
struct issuer_rec *Issuer;


//=============================================================================
// Private data definitions
//=============================================================================
static const ENTRY_STR InvoiceNumEntry = {
	EnterInvNum,				// Title
	N_NullStr,					// No default
	N_NullStr,					// Not used
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  InputAdjInv     Get and save the Adjust Invoice Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        UWORD       Transaction Journal Identifier to be adjusted
//                              0x0000 - cancel/abort the transaction.
//
//  Notes:          Make sure the entered Invoice Number exists in
//                  the Journal, and for the given Cashier (if
//                  necessary).  If presence, does read it into TRINP. 
//                  Return transaction journal identifier.
//                  AdjAgain indicates, that adjust can be call again
//-----------------------------------------------------------------------------

UWORD InputAdjInv( void )
{
    UWORD AdjIdent = 0x0000;

	// Lock the journal first
	JournalLock ();

	// See if Invoice already exists
	if ( TRINP.TRINV[0] )
	{
		// Yes; assume that we entered the Adjust Transaction via
		// Batch Review (transaction data already in TRINP)

		// Do not loop back to Invoice prompt after adjustment
		AdjAgain = False;

		// Set up selection data in common record JSREC
		SetAdjustJsrec(  );

		// JSREC will be used by JournalSelFirst()
		AdjIdent = JournalSelFirst ();

	}
	else
	{
		// Loop back to Invoice prompt after adjustment
		AdjAgain = True;

		// Do until a wanted Invoice Number is found
		do
		{
			// Notify the user of input request


			// Get an Invoice Number from the user
			CSTMSG[0] = 0;
			if ( !AlphaNumEntry( 1, S_TRINV, 1, &InvoiceNumEntry ) )
			{
				// Cancel; release the lock on the journal semaphore
				JournalRelease ();

				// Exit the function
				return 0x0000;
			}

			// Copy Invoice Number into TRINP
			MovAscZFill( ( char * ) TRINP.TRINV, Dspbuf, S_TRINV, ' ' );

			// Set up selection data in common record JSREC
			SetAdjustJsrec(  );

			// JSREC will be used by JournalSelFirst()
			AdjIdent = JournalSelFirst ();

			// See if Journal entry found
			if ( 0x0000 == AdjIdent )
			{
				// No; notify the user
				ShowErrMsg( InvNumberMissing );
			}
		}
		while ( 0x0000 == AdjIdent );

		// Read data into TRINP
		JournalRead ( AdjIdent, &TRINP );
	}

	// Release the lock on the journal semaphore
	JournalRelease ();

	// See if Adjust is allowed
	if ( ChkAdjust(  ) )
	{
		// Yes; see if Batch Pending for this Acquirer
		if ( ChkBatPend(  ) )
		{
			// Yes; notify the user of Batch Pending
			ShowErrMsg( AdjustNotAllowed );
			ShowErrMsg( RetryBatchTrans );
            return 0x0000;
        }
		else
		{
			// No; okay to continue
			return AdjIdent;
		}
	}
	else
	{
		// No; notify the user that Adjust is not allowed
		ShowErrMsg( AdjustNotAllowed );
        return 0x0000;
	}

}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  ChkAdjust       Checks to see if Adjust is allowed
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Adjust is allowed
//                              False - Adjust is not allowed.
//
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool ChkAdjust( void )
{
	Bool AdjustFlag;

	// Get Pointer to Issuer Table for this ID
	Issuer = FindIssuer( TRINP.TRISID );

	// See if Adjust is allowed
	if ( (Issuer->ISOPT1) & IS1_ADJ )
	{
		// Check for cash transaction
		if ( DBCASH == TRINP.TRKEY )
		{
			// Adjust not allowed for cash transactions
			AdjustFlag = False;
		}
		else
		{
			// Yes; set Adjust flag to true
			AdjustFlag = True;
		}
	}
	else
	{
		// No; set Adjust flag to false
		AdjustFlag = False;
	}

	// Return the flag
	return ( AdjustFlag );
}


//-----------------------------------------------------------------------------
//  SetAdjustJsrec      Sets up JSREC for Adjust Transaction
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------

static void SetAdjustJsrec( void )
{
	// Set up default selection data in common record JSREC
	SetDefaultSelection ();

	// Adjust only Cashier Entered?
	if ( TCONF.TCOPT4 & TC4_SRV_ADJ )
	{
		// Yes; move Cashier to Journal search
		memcpy( JSREC.JSCASHIER, TRINP.TRCASHIER, S_TRCASHIER );
	}

	// Move Invoice Number into Journal search
	memcpy( JSREC.JSINV, TRINP.TRINV, S_TRINV );
}
