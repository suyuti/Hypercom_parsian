
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             jrnapp.c
//      Journal reprint routines
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "sdkterm.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "journal.h"
#include "msg.h"
#include "util.h"
#include "utllib.h"
#include "chkprinter.h"
#include "totals.h"
#include "hstprint.h"
#include "receipt.h"
#include "hstprint2.h"
#include "trinpclr.h"
#include "tables.h"

#include "jrnapps.h"

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
static const ENTRY_STR InvoiceNumEntry = {
	EnterInvNum,				// Title
	N_NullStr,                  // Empty
	N_NullStr,                  // Empty
};


//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  JrnReprintAny   REPRINT a given transaction. Prompts for INV.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//-----------------------------------------------------------------------------

extern void JrnReprintAny( void )
{
	static UBYTE retval;
	static Bool loopflag;
    static UWORD JrnIdent;

	// Test for printer configured
	if ( ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Keep in loop until good invoice number entered
		loopflag = True;

		while ( loopflag )
		{
			// Prompt for invoice number to reprint
			retval = AlphaNumEntry( 1, S_TRINV, 0, &InvoiceNumEntry );

			// Abort if cancel key was pressed.
			if ( 0 == retval )
			{
				ButtonId = CANCEL_KY;
				return;
			}

			// If enter without data then display last.
			if ( 0 == StrLn( Dspbuf, sizeof( Dspbuf ) ) )
			{
				JrnReprintLast(  );
				return;
			}

			// Locate the record in the journal and then print it
			JournalLock(  );

			// Set up selection data in common record JSREC
			// Don't look for voids yet
			SetDefaultSelection(  );

			// Copy invoice number that was input to search record
			MovAscZFill( ( char * ) JSREC.JSINV, Dspbuf, S_TRINV, ' ' );

			// JSREC will be used by JournalSelFirst()
			// JrnIdent will be set with first ident, if one exists.
			JrnIdent = JournalSelFirst(  );

			if ( 0x0000 == JrnIdent )
			{
				// No records found, now look for voids
				JSREC.JSSTATOFF &= ~TS1_VOID;
				JrnIdent = JournalSelFirst(  );
			}

			if ( 0x0000 == JrnIdent )
			{
				// No entries found in the journal, void or otherwise
				ShowErrMsg( NoInvoice );
			}
			else
			{
				// Exit the loop after reading the record
				loopflag = False;

				// Read the record that was found
				JournalRead( JrnIdent, &TRINP );

				// See if issuer has receipts
				if ( (TRINP.TRISPTR->ISOPT2) & IS2_RECEIPT )
				{
					// Set the reprint flag.
					TRINP.TRREPRINT = True;

					// Call the print receipt routine
					Receipt ();
				}
				else
				{
					ShowErrMsg( InvNumberMissing );
				}
			}

			// Release the lock on the journal semaphore.
			JournalRelease(  );
		}						// end of while

		// Display "REPRINT COMPLETE" on lower line
		ShowInfoMsg( ReprintComplete, N_Reprint );

		// Notify the user
		SDK_BeepIt( 2 );

		// Be sure to clear out transaction data because could be here
		// from a menu that has a previous button.
		TrinpClear(  );

		// Delay for two seconds
		SDK_Wait( ONESECOND * 2 );
	}
}



//-----------------------------------------------------------------------------
//  JrnReprintLast  Reprint last receipt.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes: 
//-----------------------------------------------------------------------------

extern void JrnReprintLast( void )
{
	UWORD LastIdentifier, JrnIdent;

	// Test for printer configured
	if ( ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// If close batch not pending
		if ( !TFTAB.TFLAQID )
		{
			// Locate the last record printed and then print it
			JournalLock(  );

			// Set up selection data in common record JSREC
			SetDefaultSelection(  );

			// JSREC will be used by JournalSelFirst()
			// JrnIdent will be set with first ident, if one exists.
			JrnIdent = JournalSelFirst(  );

			if ( 0x0000 == JrnIdent )
			{
				// No entries in the journal
				ShowErrMsg( NoInvoice );
			}
			else
			{
				//  Set LastIdentifier to first ident before searching
				LastIdentifier = JrnIdent;

				while ( 0x0000 != JrnIdent )
				{
					// Locate the last record in the batch
					JrnIdent = JournalSelNext( JrnIdent );

					if ( 0x0000 != JrnIdent )
					{
						LastIdentifier = JrnIdent;
					}
				}				//  End of while

				// Read the record that was found
				JournalRead( LastIdentifier, &TRINP );

				// See if issuer has receipts configured
				if ( (TRINP.TRISPTR->ISOPT2) & IS2_RECEIPT )
				{
					// Set the reprint flag.
					TRINP.TRREPRINT = True;

					// Call the print receipt routine
					Receipt ();

					// Display "REPRINT COMPLETE" on lower line
					ShowInfoMsg( ReprintComplete, N_Reprint );

					// Notify the user
					SDK_BeepIt( 2 );

					// Delay for two seconds
					SDK_Wait( ONESECOND * 2 );
				}
				else
				{
					// Issuer does not have receipts configured
					ShowErrMsg( NoInvoice );
				}
			}

			// Release the lock on the journal semaphore.
			JournalRelease(  );
		}
		else
		{
			// Close batch is pending - Set up last acquirer
			TRINP.TRAQID = TFTAB.TFLAQID;
			TRINP.TRAQPTR = FindAcquirer( TRINP.TRAQID );
			if ( 0x0000 != TRINP.TRAQPTR )
			{
				// Notify user -- "PRINTING, WAIT..."
				ShowInfoMsg( PrintingWait, PleaseWait );

				// Copy TFTAB fields to TRINP for printing.
				memcpy( TRINP.TRYEAR, TFTAB.TFYEAR, S_TFYEAR );
				memcpy( TRINP.TRHOSTTEXT, TFTAB.TFHOSTTEXT, S_TRHOSTTEXT );

				// Yes; lock the Journal
				JournalLock ();

				// Set journal selection defaults for this acquirer.
				SetDefaultSelection(  );

				// Search for settled records only.
				JSREC.JSSTATOFF &= ~TS1_SETL;

				// Search for this acquirer only.
				JSREC.JSAQID = TRINP.TRAQID;

				// Calculate totals for this acquirer.
				CalcTotals_Jsrec ();

				// Set the reprint flag.
				TRINP.TRREPRINT = True;

				// Release the lock on the journal semaphore.
				JournalRelease(  );

				// Print the settle and issuer receipts.
				SettleReceipt ();
				IssuerReceipt ();

				// Display "REPRINT COMPLETE" on lower line
				ShowInfoMsg( ReprintComplete, BlankMsg );

				// Notify the user
				SDK_BeepIt( 2 );

				// Delay for two seconds
				SDK_Wait( ONESECOND * 2 );
			}
			else
			{
				// No entries in the journal
				ShowErrMsg( NoInvoice );
			}
		}

		// Be sure to clear out transaction data because could be here
		// from a menu that has a previous button.
		TrinpClear(  );
	}
}

//=============================================================================
// Private function definitions
//=============================================================================
