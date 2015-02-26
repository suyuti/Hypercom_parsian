
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             tabclose.c
//      Functions related to CLOSE TAB transaction
//
//=============================================================================

#include "string.h"

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
#include "findtran.h"
#include "journal.h"
#include "disproc.h"
#include "receipt.h"
#include "respdsp.h"
#include "inputpwd.h"
#include "amtentry.h"
#include "adjust.h"
#include "inadjinv.h"
#include "cstmamnt.h"
#include "tables.h"
#include "isrest.h"
#include "cndmtamt.h"
#include "inadjinv.h"

#include "tabclose.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static UBYTE InputCtabTotal( void );
static Bool InputCtabBase( void );
static Bool InputCtabBar( void );
static Bool InputCtabTax( void );
static Bool InputCtabTip( void );
static Bool CompleteCloseTab( UWORD ClTabJrIdent );
static void CmplCloseTabAmts( void );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static const ENTRY_STR YNEntry =	// structure for YesNoEntry() 
{
	N_NullStr,
	CustomMsg,
	CorrectYesNo,
};

static const ENTRY_STR AmtEntry =	// structure for AmountEntry() 
{
	N_NullStr,
	CustomMsg,
	AmountNew,
};



//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  CloseTabProc
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          This function must be in the same page as
//                  adjust and review transactions
//
//-----------------------------------------------------------------------------

extern void CloseTabProc( void )
{
	char loopflag;
    UWORD MyJrnIdent;

	TRINP.TRKEY = CLOSETAB;

	// Save and verify the Input Password if needed 
	// See if Password is needed 
	if ( !( TCONF.TCLOCOP & TCL_NO_APWD ) )
	{
		// Yes; save and verify the Input Password 
		if ( !InputPswd( N_NullStr ) )
			return;
	}

	// Get the Cashier Number allowing NULL entry 
	
	// Only allow retreive of OPEN TAB transactions.    
	loopflag = True;

	while ( loopflag )
	{
		// Get the invoice number of the tab to be closed.
        // and copy transaction into TRINP  
        MyJrnIdent = InputAdjInv(  );
		if ( 0x0000 == MyJrnIdent )
			return;

		// Verify that transaction selected is an OPENTAB.      
		// Adjust of non open tab transactions is not allowed.  
		if ( OPENTAB == TRINP.TRKEY )
		{
			// Exit the loop - open tab selected.   
			loopflag = False;
		}
		else
		{
			// Clear invoice number and allow reentry.  
			memset( TRINP.TRINV, 0,S_TRINV );

			ShowErrMsg( NotOpenTab );
		}
	}

	// Continue close at same point that is called by adjust. 
	if ( CloseTabCommon( MyJrnIdent ) )
	{
		// Display Response from the transaction. 
		ShowIssuer_Tran(  );
		RespDisplay ();
	}
}

//-----------------------------------------------------------------------------
//  CloseTabCommon
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Bool    True  - if successful,
//                          False - otherwise.
//
//  Notes:          This function must be in the same page as
//                  adjust and review transactions.
//
//-----------------------------------------------------------------------------

Bool CloseTabCommon( UWORD TabCloseJrnIdent )
{
	UBYTE retval, length;
	UBYTE Status;
	int char_limit;

	// Display issuer and transaction.              
	// Use second event because may be in review.   
	SecondEvent.TRNKEY = CLOSETAB;
	FindTran ();

	// Calculate maximum number of characters to display on a line, in screen_limit 
	MaxCharDisp ();

	char_limit = screen_limit;

	// Space fill the global custom message buffer 
	memset( CSTMSG, ' ',( UWORD ) char_limit );

	// Null terminate the custom message buffer 
	CSTMSG[char_limit] = '\0';

	// Get the length of the Transaction Name 
	length =
		StrLn( ( char * ) SecondEvent.TRNTEXT,
			   sizeof( SecondEvent.TRNTEXT ) );

	// Move the Transaction Name (Right-Justified) to message 
	memcpy( &CSTMSG[char_limit - length], SecondEvent.TRNTEXT, length );

	// Put Issuer Name into the Custom Message buffer 
	MoveItIss( CSTMSG, ( char * ) TRINP.TRISPTR->ISNAME, S_ISNAME );

	// Verify that user wants to close a tab. 
	retval = YesNoEntry( &YNEntry );
	if ( retval != ENTER_KY )
		return ( False );

	// If cashier data not on read record then prompt for it. 
	if ( NullComp( ( char * ) TRINP.TRCASHIER, S_TRCASHIER ) )
	{
		// Get the Cashier Number.  Null entry not allowed. 
		
	}

	// Locally save variable data.  
	CopyAmounts(  );

	// Process until the Total Amount is what user wants 
	while ( True )
	{
		// Get Total Amount 
		Status = InputCtabTotal(  );

		// Check the status 
		if ( !Status )
		{
			// Cancel; exit the function 
			return ( False );
		}

		// Check if input steps to continue 
		if ( 1 == Status )
		{
			// Yes; get the Base Amount 
			if ( !InputCtabBase(  ) )
			{
				// Cancel; exit the function 
				return ( False );
			}

			// Get the Bar Amount 
			if ( !InputCtabBar(  ) )
			{
				// Cancel; exit the function 
				return ( False );
			}

			// Get the Tax Amount 
			if ( !InputCtabTax(  ) )
			{
				// Cancel; exit the function 
				return ( False );
			}

			// Get the Tip Amount 
			if ( !InputCtabTip(  ) )
			{
				// Cancel; exit the function 
				return ( False );
			}
		}
		else
		{
			// No; exit the input loop 
			break;
		}
	}

	// Complete the close tab processing.           
	// Update the journal and print the receipt.    
	CompleteCloseTab( TabCloseJrnIdent );
	
    return ( True );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  InputCtabTotal  Input Close Tab Total.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        UBYTE   0 - cancel/abort the Transaction,
//                          1 - okay to continue,
//                          2 - complete the Transaction now.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static UBYTE InputCtabTotal( void )
{
	UBYTE status;

	// Move the Total Amount into the global custom message 
	CustomAmount( AmountTotals, NEWTOTAM );

	// See if this is the correct Amount 
	switch ( YesNoEntry( &YNEntry ) )
	{
		case 0:
		case CANCEL_KY:		// CANCEL key pressed 

			// Return cancel/abort the Transaction 
			status = 0;
			break;

		case ENTER_KY:			// YES key was pressed 

			// Return complete the Transaction now 
			status = 2;
			break;

		default:				// No key was pressed   

			// Return to get other amounts. 
			status = 1;
			break;
	}

	return ( status );
}


//-----------------------------------------------------------------------------
//  InputCtabBase   Get and save the Close TAb Base Amount.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Bool    True  - okay to continue,
//                          False - cancel/abort the transaction.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static Bool InputCtabBase( void )
{
	UBYTE Status;

	// Add check for Nova Host here 

	// Do we prompt for multiple Amounts ? 
	if ( !CndMultAmt(  ) )
	{
		// No; return okay 
		return ( True );
	}

	// Loop till done 
	do
	{
		// Move the Base Amount into the global custom message 
		CustomAmount( AmountBase, NEWBASE );

		// Do until done 
		do
		{
			// Set clear key 
			GlobalDsp = CLRKEYON;

			// Get new Base Amount 
			switch ( AmountEntry( &AmtEntry, 0, 0 ) )
			{
				case 1:		// See if new Base Amount is valid 
					if ( SaveAdjust( NEWBASE ) )
					{
						// Yes; set status to indicate okay 
						Status = 1;
					}
					else
					{
						// No; set status to redo input 
						Status = 88;
					}

					break;

				case 3:		// Go back to previous entry 
					Status = 99;

					break;

				default:		// Entry canceled 
					Status = 0;

					break;
			}
		}
		while ( 88 == Status );
	}
	while ( 99 == Status );

	// Return the status 
	return ( Status );
}


//-----------------------------------------------------------------------------
//  InputCtabBar    Get and save the Close Tab Bar Amount.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Bool    True  - okay to continue,
//                          False - cancel/abort the transaction.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static Bool InputCtabBar( void )
{
	UBYTE Status;

	// Loop till done 
	do
	{
		// Move the Bar Amount into the global custom message 
		CustomAmount( AmountBar, NEWBAR );

		// Do until done 
		do
		{
			// Set clear key 
			GlobalDsp = CLRKEYON;

			// Get new Bar Amount 
			switch ( AmountEntry( &AmtEntry, 1, 0 ) )
			{
				case 1:		// See if new Bar Amount is valid 
					if ( SaveAdjust( NEWBAR ) )
					{
						// Yes; set status to indicate okay 
						Status = 1;
					}
					else
					{
						// No; set status to redo input 
						Status = 88;
					}

					break;

				case 3:		// Go back to previous entry 
					Status = 99;

					break;

				default:		// Entry canceled 
					Status = 0;

					break;
			}
		}
		while ( 88 == Status );
	}
	while ( 99 == Status );

	// Return the status 
	return ( Status );
}


//-----------------------------------------------------------------------------
//  InputCtabTax    Get and save the Close Tab Tax Amount.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Bool    True  - okay to continue,
//                          False - cancel/abort the transaction.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------
// kadlýr @a
static Bool InputCtabTax( void )
{
	UBYTE Status;
        
        return ( True );
	

	// Loop till done 
	do
	{
		// Move the Tax Amount into the global custom message 
		CustomAmount( AmountTax, NEWTAX );


		// Do until done 
		do
		{
			// Set clear key 
			GlobalDsp = CLRKEYON;

			// Get new Tax Amount 
			switch ( AmountEntry( &AmtEntry, 1, 0 ) )
			{
				case 1:		// See if new Tax Amount is valid 
					if ( SaveAdjust( NEWTAX ) )
					{
						// Yes; set status to indicate okay 
						Status = 1;
					}
					else
					{
						// No; set status to redo input 
						Status = 88;
					}

					break;

				case 3:		// Go back to previous entry 
					Status = 99;

					break;

				default:		// Entry canceled 
					Status = 0;

					break;
			}
		}
		while ( 88 == Status );
	}
	while ( 99 == Status );

	// Return the status 
	return ( Status );
}


//-----------------------------------------------------------------------------
//  InputCtabTip    Get and save the Close Tab Tip Amount.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Bool    True  - okay to continue,
//                          False - cancel/abort the transaction.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static Bool InputCtabTip( void )
{
	
}


//-----------------------------------------------------------------------------
//  CompleteCloseTab    All the new Totals have been entered, now it is
//                      time to store the changes in the Journal
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Bool    True  - okay to continue,
//                              False - cancel/abort the transaction.
//
//  Notes:              None.
//
//-----------------------------------------------------------------------------

static Bool CompleteCloseTab( UWORD ClTabJrIdent )
{
	Bool Status;
	TRINP.TRKEY = SALE;

	// Get control of the Journal 
	JournalLock ();

	// Update Amounts and flags 
	CmplCloseTabAmts(  );

	// Update the Journal 
	ClTabJrIdent = JournalUpdate ( &TRINP, ClTabJrIdent );

	// Get the status 
	if ( 0x0000 == ClTabJrIdent )
	{
		Status = False;
	}
	else
	{
		Status = True;

	}

	// Release the Journal 
	JournalRelease ();

	// See if status okay 
	if ( Status )
	{
		// Yes; notify the user 
		// Display "TRANSACTION ACCEPTED" on upper line 
		ShowInfoMsg( TransAccepted, N_Response );

		// Notify the user 
		SDK_BeepIt( 2 );

		// Change transaction key back CLOSETAB 
		// just for the receipt print.          
		TRINP.TRKEY = CLOSETAB;
		Receipt ();

		// Change transaction key back to SALE  ( or SALOFFL if FDR )
		// in case we are in review.            
		TRINP.TRKEY = SALE;
	}
	else
	{
		// No; notify the user 
		ShowErrMsg( InvNumberMissing );
	}

	// Return the status 
	return ( Status );
}


//-----------------------------------------------------------------------------
//  CmplCloseTabAmts    Complete close tab amounts.
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:              None.
//
//-----------------------------------------------------------------------------

static void CmplCloseTabAmts( void )
{
	// See if original Amount set already 
	if ( NullComp( ( char * ) TRINP.TRORIGAMT, S_TRBASE ) )
	{
		// No; see if Advice outstanding already 
		if ( !( TRINP.TRSTATUS[0] & ( TS1_ADV + TS1_ADJ ) ) )
		{
			// No; save old Total 
			memcpy( TRINP.TRORIGAMT, TRINP.TRTOTAM, S_TRBASE );
		}
	}

	// Move from work area back to Journal - Total Amount 
	memcpy( TRINP.TRTOTAM, NEWTOTAM, S_TRTOTAM );

	// Copy the new Base Amount into the Transaction's Base Amount
	// Field so it can be printed on the Receipt (it is not stored in
	// the Journal) 
	memcpy( TRINP.TRBASE, NEWBASE, S_TRBASE );

	// Move from work area back to Journal - Tax Amount 
	memcpy( TRINP.TRTAX, NEWTAX, S_TRBASE );

	// Move from work area back to Journal - Tip Amount 
	memcpy( TRINP.TRTIP, NEWTIP, S_TRTIP );

	// Move from work area back to Journal - Bar Amount 
	memcpy( TRINP.TRBAR, NEWBAR, S_TRBASE );

	// Mark as Adjusted 
	TRINP.TRSTATUS[0] |= TS1_ADJ;

}
