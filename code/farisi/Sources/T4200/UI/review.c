//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      review.c
//
// File Description:
//      Batch review screen operations
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdktime.h"
#include "fixdata.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "adjust.h"
#include "tables.h"
#include "chkprinter.h"
#include "comdata.h"
#include "cvtamt.h"
#include "datetime.h"
#include "disproc.h"
#include "findtran.h"
#include "inadjinv.h"
#include "journal.h"
#include "msg.h"
#include "receipt.h"
#include "review.h"
#include "util.h"
#include "void.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
//! Process common part of review and find
static void ReviewCommon( UWORD ReviewIdent );
//! Display 2 lines of information, group 1, 2, or 3
static void JournalShow( UBYTE group );
//! Format the batch number display into CSTMSG
static void ShowBatch( void );
//! Test for password supervisor required
static Bool SupervisorPassword( UBYTE local_option, UBYTE transaction_name );


//=============================================================================
// Public data definitions
//=============================================================================

//=============================================================================
// Private data definitions
//=============================================================================
//! Supervisor password is entered or not yet
static UBYTE Supv_Password;

//! Invoice number entry
static const ENTRY_STR InvoiceNumEntry = {
	//! Title
	EnterInvNum,
	//! No default
	N_NullStr,
	//! Not used
	N_NullStr,					
};

//! Supervisor password entry
static const ENTRY_STR SupvPasswdEntry = {
	//! Title
	N_NullStr,
	//! Prompt
	EnterPswd,
	//! Default value
	N_NullStr,					
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!      Process review transaction
//!
//! \param
//!				
//! \return
//!
//! \note
//-----------------------------------------------------------------------------
extern void Review( void )
{
    UWORD ReviewJrnIdent, tmpIdent;

	// Set Transaction Type in TRINP
	TRINP.TRKEY = REVIEW;

	// Get Cashier Number allowing NULL entry
	if ( !InputCashier( CustomMsg, True ) )
	{
		// User cancel; return
		return;
	}

	// Locate the last record
	JournalLock ();

	// Set up selection data in common record JSREC
	SetDefaultSelection ();

	// Include voids and reversals in the search, but not settled.
	JSREC.JSSTATOFF = TS1_SETL;

	// Test configuration to see if batch review cashier entered.
	if ( TCONF.TCOPT4 & TC4_SRV_BAT )
	{
		// Move cashier to journal search
		memcpy( JSREC.JSCASHIER, TRINP.TRCASHIER, S_TRCASHIER );
	}

	// JSREC will be used by JournalSelFirst()
	// tmpIdent will be set with first ident, if one exists.
	tmpIdent = JournalSelFirst ();

	if ( 0x0000 == tmpIdent )
	{
		// Release the lock on the journal before exiting.
		JournalRelease ();

		// No entries in the journal
		ShowBatch(  );			// Get batch number into CSTMSG
		ShowInfoMsg( CustomMsg, StartOfBatch );
		SDK_Wait( ONESECOND * 2 );
	}
	else
	{
		//  Set LastIdentifier to first ident before searching
		ReviewJrnIdent = tmpIdent;

		while ( 0x0000 != tmpIdent )
		{

			// Locate the last record in the batch
			tmpIdent = JournalSelNext ( tmpIdent );
			if ( 0x0000 != tmpIdent )
			{
				ReviewJrnIdent = tmpIdent;
			}
		}						//  End of while

		// Go do actual review starting with last entry in journal.
		ReviewCommon( ReviewJrnIdent );
	}
}

//-----------------------------------------------------------------------------
//!		Process review transaction starting with a
//!		particular invoice numberDescription line 2
//!		Cross called from TCF page or FUN page.
//!
//! \param
//!				
//! \return
//!
//! \note
//-----------------------------------------------------------------------------
extern void Find( void )
{
	static Bool loopflag;
    UBYTE FindJrnIdent;

	// Prompt for invoice to start with
	// Keep in loop until good invoice number entered
	loopflag = True;

	while ( loopflag )
	{
		//not really necessary now that AlphaNumEntry is fixed?
		//SDK_GuiClearScr();  // Clean up button table

		// Abort if cancel key is pressed.
		if ( 0 == ( AlphaNumEntry( 1, S_TRINV, 1, &InvoiceNumEntry ) ) )
		{
			// Abort if cancel key was pressed.
			return;
		}

		// Locate the record in the journal
		JournalLock ();

		// Set up selection data in common record JSREC
		SetDefaultSelection ();

		// Include voids and reversals in the search, but not settled.
		JSREC.JSSTATOFF = TS1_SETL;

		// Copy invoice number that was input to search record
		MovAscZFill( ( char * ) JSREC.JSINV, Dspbuf, S_TRINV, ' ' );

		// JSREC will be used by JournalSelFirst()
		// FindJrnIdent will be set with first ident, if one exists.
		FindJrnIdent = JournalSelFirst ();

		if ( 0x0000 != FindJrnIdent )
		{
			// Entry found in the journal.
			loopflag = False;
		}
		else
		{
			// Release the lock on the journal semaphore.
			// The user may press cancel and exit.
			JournalRelease ();

			ShowErrMsg( InvNumberMissing );
		}

	}							// End of while loop

	// Clear invoice number from search record
	memset( JSREC.JSINV, 0,S_TRINV );

	// Go do actual review starting with the entered invoice.
	ReviewCommon( FindJrnIdent );
}



//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!		Process common part of review and find.
//!
//! \param
//!				
//! \return
//!
//! \note
//-----------------------------------------------------------------------------
static void ReviewCommon( UWORD ReviewIdent )
{
	UBYTE Key;
	UBYTE group;
    UWORD CurrentIdent, JournalIDSave;
	struct jrnfind_rec REVIEW_JSREC;	// Journal select record

	// Preset supervisor password not entered yet.
	Supv_Password = False;

	// Test configuration to see if batch review cashier entered.
	if ( TCONF.TCOPT4 & TC4_SRV_BAT )
	{
		// Move cashier to journal search
		memcpy( JSREC.JSCASHIER, TRINP.TRCASHIER, S_TRCASHIER );
	}

	// Set up for read of records.
	CurrentIdent = ReviewIdent;

	Enable_Flags[1] = 0;
	Enable_Flags[2] = 0;
	Enable_Flags[3] = 0;
	Enable_Flags[4] = 0;

	// Loop displaying journal records until cancel is pressed
	while ( True )
	{

		Enable_Flags[1] = 1; //use first screen
		Enable_Flags[2] = 0;
		Enable_Flags[3] = 0;
		Enable_Flags[4] = 0;

		// Read the entire selected journal record into TRINP.
		JournalRead ( CurrentIdent, &TRINP );

		// Set the staring line for this screen
		group = 1;

		// Loop here displaying each group of information.
		do
		{

			JournalShow( group );

			DrawScreen( REVIEW_SCR );	// Display Review Screen

			// Get a key from the user
			Key = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

			// Check if cancel or Time-Out
			if ( ( CANCEL_KY == Key ) || ( 0 == Key ) )
			{
				// Yes; quit the loop
				break;
			}

			// No; base action upon the key press
			switch ( Key )
			{
				case B_PREVIOUS:
//				case CLEAR_KY:
					// Go to previous transaction in the journal.
					CurrentIdent = JournalSelPrevious ( CurrentIdent );

                    if ( 0x0000 == CurrentIdent )
					{
						// No next record. Display Start and batch #.
						ShowBatch(  );	// Get batch number into CSTMSG
						ShowInfoMsg( CustomMsg, StartOfBatch );
						SDK_Wait( TENMS * 100 );

						// Restore previously displayed entry ID.
						CurrentIdent = ReviewIdent;
					}
                    else
                    {
						// Apply new displayed entry ID.
						ReviewIdent = CurrentIdent;
                    }

                    group = 1;
					break;

                case B_NEXT:
					// Go to next transaction in the journal.
					CurrentIdent = JournalSelNext ( CurrentIdent );
					if ( 0x0000 == CurrentIdent )
					{
						// No next record. Display End and batch #.
						ShowBatch(  );	// Get batch number into CSTMSG
						ShowInfoMsg( CustomMsg, EndOfBatch );
						SDK_Wait( TENMS * 100 );


						// Restore previously displayed entry ID.
						CurrentIdent = ReviewIdent;
					}
					else
					{
						ReviewIdent = CurrentIdent;
					}
					group = 1;
					break;

				case ENTER_KY:	// ENTER key was pressed

					Enable_Flags[1] = 0;
					Enable_Flags[2] = 1; //scroll transaction data screen
					Enable_Flags[3] = 0;
					Enable_Flags[4] = 0;

					group = 1;
					Key = 0xff;
					break;

				case 'D':	// perform Action

					Enable_Flags[1] = 0;
					Enable_Flags[2] = 0; 
					Enable_Flags[3] = 1;
					Enable_Flags[4] = 0;

					group = 1;
					Key = 0xff;
					break;

				case 'S':		// SCROLL key was pressed
					// Increment group - wrap to 1 if over 3
					group++;
					if ( 2 < group )
						group = 1;

					// Stay in same journal record if scrolling.
					Key = 0xff;
					break;

				case 'A':		// ADJUST key was pressed
					if ( !( TRINP.TRSTATUS[0] & TS1_VOID )
						 && SupervisorPassword( TCL_NO_APWD, ADJUST ) )
					{
						// Save JSREC locally thru possible host call.
						memcpy( &REVIEW_JSREC, &JSREC, JSSZE );
						JournalIDSave = ReviewIdent;

						// Release the lock on the journal semaphore.
						JournalRelease ();


						// Perform Adjust with knovn Invoice (transaction in TRINP)
						GetMsg( N_Adjust, DefaultTitle );	// Override title to show ADJUST
						DoAdjust(  );
						DefaultTitle[0] = 0;	// Clean up

						// Restore JSREC and lock the journal again.
						JournalLock ();
						ReviewIdent = JournalIDSave;
						memcpy( &JSREC, &REVIEW_JSREC, JSSZE );
					}

					// Stay in same journal record
					// but force to reread from the journal.
					Key = 0xfe;
					group = 1;

					if ( CANCEL_KY == ButtonId )
						Key = CANCEL_KY;

					Enable_Flags[1] = 1;
					Enable_Flags[2] = 0; 
					Enable_Flags[3] = 0;
					Enable_Flags[4] = 0;

					break;

				case 'R':		// REPRINT key was pressed

					// Test for printer configured
					if ( ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
					{

						// See if issuer has receipts
						if ( (TRINP.TRISPTR->ISOPT2) & IS2_RECEIPT )
						{
							// Set the reprint flag.
							TRINP.TRREPRINT = True;

							// Call the print receipt routine
							Receipt ();
						}
					}

					// Stay in same journal record but force to reread
					// from the journal. After reprint TRINP is cleared.
					Key = 0xfe;
					group = 1;
					Enable_Flags[1] = 1;
					Enable_Flags[2] = 0; 
					Enable_Flags[3] = 0;
					Enable_Flags[4] = 0;

					break;

				case 'V':		// VOID key was pressed
					if ( !( TRINP.TRSTATUS[0] & TS1_VOID )
						 && SupervisorPassword( TCL_NO_VPWD, VOIDTRAN ) )
					{
						// Save JSREC locally thru possible host call.
						memcpy( &REVIEW_JSREC, &JSREC, JSSZE );
						JournalIDSave = ReviewIdent;

						// Release the lock on the journal semaphore.
						JournalRelease ();

						GetMsg( N_Void, DefaultTitle );
						VoidIdent( ReviewIdent );
						DefaultTitle[0] = 0;

						// Lock the journal again and restore JSREC.
						JournalLock ();
						memcpy( &JSREC, &REVIEW_JSREC, JSSZE );
						ReviewIdent = JournalIDSave;
					}

					// Stay in same journal record
					// but force to reread from the journal.
					Key = 0xfe;
					group = 1;

					Enable_Flags[1] = 1;
					Enable_Flags[2] = 0; 
					Enable_Flags[3] = 0;
					Enable_Flags[4] = 0;

					break;
			}
		}
		while ( 0xff == Key );

		// Check for User cancel or no totals
		if ( ( CANCEL_KY == Key ) || ( 0 == Key ) )
		{
			// Yes; quit the main while loop
			break;
		}
	}

	// Release the lock on the journal semaphore.
	JournalRelease ();

	// Void process leaves the DelayIdleDisplay flag True.  In
	// all cases, reset flag to not leave response on display.
	DelayIdleDisplay = False;
}

//-----------------------------------------------------------------------------
//!		Format the batch number display into CSTMSG.
//!
//! \param
//!				
//! \return
//!
//! \note
//-----------------------------------------------------------------------------
static void ShowBatch( void )
{
	struct batch_rec *pBatch_Table;	// Pointer to BATTAB entry
	UBYTE aqid;

	// Set the display buffer to all spaces.
	memset( CSTMSG, ' ', sizeof( CSTMSG ) );

	// Display current batch number
	if ( TRINP.TRAQID )
	{
		aqid = TRINP.TRAQID;
	}
	else
	{
		// No journal records, default to first acquirer.
		aqid = AQTAB[0].AQID;
	}

	pBatch_Table = FindBatTab( aqid );
	BfAscii( ( char * ) &CSTMSG[14], pBatch_Table->BATCUR, S_BATCUR );
	CSTMSG[14 + ( S_BATCUR * 2 )] = '\0';

}

//-----------------------------------------------------------------------------
//!		Display 2 lines of information, group 1, 2, or 3.
//!
//! \param
//!		group		group number, 1, 2 or 3
//!				
//! \return
//!
//! \note
//-----------------------------------------------------------------------------
static void JournalShow( UBYTE group )
{
	UBYTE i;
	UBYTE length;

	// Calculate maximum number of characters to display on a line, in screen_limit
	MaxCharDisp ();

	// Set the display buffers to all spaces.
	memset( CSTMSG, ' ', sizeof( CSTMSG ) );
	memset( CSTMSG2, ' ', sizeof( CSTMSG ) );
	memset( CSTMSG3, ' ', sizeof( CSTMSG ) );
	memset( CSTMSG4, ' ', sizeof( CSTMSG ) );

	// VOID button visible by default
	Enable_Flags[0] = 0;

	// Display either cashier group, approval group or seq. group
	if ( 1 == group )
	{
		// Build and display the first group, the cashier group.
		if ( !NullComp( ( char * ) TRINP.TRCASHIER, S_TRCASHIER ) )
		{
			// Test option to treat cashier as server
			if ( TCONF.TCOPT4 & TC4_SRV_PRM )
			{
				GetMsg( Msg_SVR, CSTMSG );
				length = StrLn( CSTMSG, sizeof( CSTMSG ) );
			}
			else
			{
				GetMsg( Msg_CSH, CSTMSG );
				length = StrLn( CSTMSG, sizeof( CSTMSG ) );
			}

			// Add in the Server/Cashier number and adjust offset
			MoveTillZero( &CSTMSG[length], ( char * ) TRINP.TRCASHIER,
						  S_TRCASHIER );
			length += S_TRCASHIER;
		}

		// Get the Invoice prompt and value and add it to display buffer
		GetMsg( Msg_INV, CSTMSG2 );
		length = StrLn( CSTMSG2, sizeof( CSTMSG2 ) );
		memcpy( &CSTMSG[( screen_limit - length - S_TRINV )], CSTMSG2,
				( UWORD ) length );
		memcpy( &CSTMSG[( screen_limit - S_TRINV )], TRINP.TRINV, S_TRINV );

		// Set up the second line of group 1
		memset( CSTMSG2, ' ',sizeof( CSTMSG2 ) );

		// Get the transaction descriptive text and options
		SecondEvent.TRNKEY = TRINP.TRKEY;
		FindTran ();

		// Right justify the transaction amount
		CvtAmt( ( char * ) &CSTMSG3[screen_limit - 13], TRINP.TRTOTAM );

		if ( ( SecondEvent.TRNOPT1 & T1_CREDIT ) ||
			 ( TRINP.TRSTATUS[0] & TS1_VOID ) )
		{
			for ( i = 0; i < screen_limit; i++ )
			{
				// Replace $ with - if refund or void type
				if ( TCONF.TCLEADC[0] == CSTMSG3[i] )
				{
					if ( ' ' == CSTMSG3[i - 1] )
						CSTMSG3[i - 1] = '-';
					else
						CSTMSG3[i] = '-';
					break;
				}
			}
		}

		// Move transaction name to display buffer
        i = StrLn( SecondEvent.TRNTEXT, sizeof( SecondEvent.TRNTEXT ) );
		memcpy( &CSTMSG2[( screen_limit - i )], SecondEvent.TRNTEXT, i );

		// Display reversal or voided if in journal status
		if ( TRINP.TRSTATUS[0] & TS1_REV )
		{
			// Is a reversal
			GetMsg( Msg_reversal, CSTMSG2 );
			length = StrLn( CSTMSG2, sizeof( CSTMSG2 ) );
			CSTMSG2[length] = ' ';

            // Hide ADJUST and VOID buttons
			if ( 1 == Enable_Flags[3] )
            {
                Enable_Flags[1] = 0;
        		Enable_Flags[2] = 0; 
            	Enable_Flags[3] = 0;
                Enable_Flags[4] = 1;
            }
		}
		else if ( TRINP.TRSTATUS[0] & TS1_VOID )
		{
			// Is voided
			GetMsg( Msg_voided, CSTMSG2 );
			length = StrLn( CSTMSG2, sizeof( CSTMSG2 ) );
			CSTMSG2[length] = ' ';
			
            // Hide ADJUST and VOID buttons
			if ( 1 == Enable_Flags[3] )
            {
                Enable_Flags[1] = 0;
        		Enable_Flags[2] = 0; 
            	Enable_Flags[3] = 0;
                Enable_Flags[4] = 1;
            }
		}

        else if ( TRINP.TRSTATUS[0] & TS1_ADV )
		{
			// Is advice
			GetMsg( Msg_advice, CSTMSG2 );
			length = StrLn( CSTMSG2, sizeof( CSTMSG2 ) );
			CSTMSG2[length] = ' ';
		}


	}
	else
	{
		// Build and display the second group, the approval group.
		// Transfer response text to display buffer
		MoveTillZero( CSTMSG, ( char * ) TRINP.TRRSPTXT, S_TRRSPTXT );

		// Set up the second line of group 1
		memset( CSTMSG2, ' ',sizeof( CSTMSG ) );

		// Display the primary account number.
		MovPanAsc( CSTMSG2, TRINP.TRPAN, ( UBYTE ) screen_limit );

		// Build and display the first line of the sequence group.
		GetMsg( Msg_SequenceNo, CSTMSG3 );
		length = StrLn( CSTMSG3, sizeof( CSTMSG3 ) );
		BfAscii( &CSTMSG3[length], TRINP.TRSTAN, S_TRSTAN );

		// Build and display the SECOND line of the third group
		// RptDateTime returns formatted date and time in CSTMSG5
		RptDateTime ();
		memcpy( &CSTMSG4[0], &CSTMSG5[0], 10 );
		memcpy( &CSTMSG4[15], &CSTMSG5[11], 5 );

	}

	// Null terminate the 1-nd display line
	CSTMSG[screen_limit] = '\0';

	// Null terminate the 2-nd display line
	CSTMSG2[screen_limit] = '\0';

	// Null terminate the 3-rd display line
	CSTMSG3[screen_limit] = '\0';

	// Null terminate the 4-th display line
	CSTMSG4[screen_limit] = '\0';
}

//-----------------------------------------------------------------------------
//!		Test for password supervisor required.
//!
//! \param
//!		local_option		local password option for this transaction
//! \param
//!		transaction_name	this transaction name
//!					
//! \return
//!		Bool   True  - if password OK or previously entered.
//!            False - if void or reversal set in journal or no pswd.
//!
//! \note
//-----------------------------------------------------------------------------
static Bool SupervisorPassword( UBYTE local_option, UBYTE transaction_name )
{
	UBYTE status;
	Bool retval = True;

	// Return True if password was entered once already.
	if ( !Supv_Password )
	{
		// Password not entered yet.  No changes to voids or reversals.
		if ( TRINP.TRSTATUS[0] & ( TS1_VOID + TS1_REV ) )
		{
			retval = False;
		}
		else
		{
			// Test no password required by local option setting.
			if ( !( TCONF.TCLOCOP & local_option ) )
			{
				// Do until correct Password entered or cancel
				while ( True )
				{
					// Prompt for Input Password (4 Digits)
					status = NumEntry( 2, 4, 4, &SupvPasswdEntry );

					// Check if Input Password has been entered
					if ( status == 1 )
					{
						// Yes; save the Input Password entered
						memcpy( TRINP.TRPWD, Dspbuf, 4 );

						// Verify Input Password entered
						if ( memcmp( TRINP.TRPWD, (UBYTE *)TERM.TERMPwd, 4 ) == 0 )
						{
							// Correct - exit while loop.
							Supv_Password = True;
							break;
						}
						else
						{
							// Password incorrect, notify user
							SDK_Beeper( TENMS * 40 );
						}
					}
					else
					{
						// No; cancel, exit the loop
						retval = False;
						break;
					}
				}				// End of while loop
			}
		}
	}
	return ( retval );
}

