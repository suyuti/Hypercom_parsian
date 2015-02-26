
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             util2.c
//      General Util functions.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "fixdata.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "comdata.h"
#include "transdef.h"
#include "journal.h"
#include "util.h"
#include "tables.h"
#include "readdata.h"

#include "util2.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static void IncTermInv( void );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static const ENTRY_STR InDupYNEntry =	// structure for YesNoEntry() 
{
	N_NullStr,
	DuplicateTrans,
	AddYesNo,
};

static const ENTRY_STR InvoiceNumEntry = {
	EnterInvNum,				// Title 
	N_NullStr,					// No default 
	N_NullStr,					// Not used 
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  InInvoiceNum    Input invoice number or use default from terminal.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        UBYTE   1 - successful,
//                          0 - otherwise.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

UBYTE InInvoiceNum( void )
{
	UBYTE retval;
	static Bool zero_flag, loop_flag;
	static UBYTE i;
	static UBYTE invoice_num[S_TRINV/2];
    static UWORD InvJrnIdent;

	loop_flag = True;

    // Check if Invoice Number manual entry required
	if ( (TRINP.TRISPTR->ISOPT2) & IS2_ROC )
	{
        // Yes; ask for Invoice Number
		while ( loop_flag )
		{
			// Read the invoice from the keyboard and test for a    
			// duplicate.  Data is stored at Dspbuf                 
			GlobalDsp = 0;
			ClrCstMsgs(  );
			retval = AlphaNumEntry( 1, S_TRINV, 1, &InvoiceNumEntry );

			// Abort if cancel key was pressed. 
			if ( 0 == retval )
			{
				loop_flag = False;
				retval = 1;
			}
			else
			{
				// Do not allow all 0's in the invoice number 
				zero_flag = False;
				for ( i = 0; i < StrLn( Dspbuf, sizeof( Dspbuf ) ); i++ )
				{
					if ( '0' != Dspbuf[i] )
					{
						// At least one non-zero was found 
						zero_flag = True;
					}
				}

				if ( zero_flag )
				{
					// Set up common selection record JSREC for journal search  
					JournalLock ();

					// Set up selection data in common record JSREC         
					SetDefaultSelection ();

					// Status bits set off 
					JSREC.JSSTATOFF = TS1_VOID + TS1_REV + TS1_SETL;

					MovAscZFill( ( char * ) JSREC.JSINV, Dspbuf, S_TRINV,
								 ' ' );

					// Find first record that has this invoice number   
					// InvJrnIdent will be set by JournalSelFirst     
					InvJrnIdent = JournalSelFirst ();
					if ( 0x0000 == InvJrnIdent )
					{
						// OK, Duplicate INVOICE was not found 
						loop_flag = False;
						retval = 0;
					}
					JournalRelease ();
				}
			}
		}
		MovAscZFill( ( char * ) TRINP.TRINV, Dspbuf, S_TRINV, ' ' );
	}

    //  No manual entry required; get Invoice Number from terminal
	else
	{
		// Assign a training/real invoice number and test duplicate  
		while ( loop_flag )
		{
			// Increment training invoice or real invoice number 
			if ( TCONF.TCOPT4 & TC4_TRAINING )
			{
				Bcd3( TRAININV );
				memcpy( invoice_num, TRAININV, sizeof( TRAININV ) );
			}
			else
			{
				// Increment terminal invoice number 
				IncTermInv(  );
				memcpy( invoice_num, (UBYTE *)TERM.TERMInv, sizeof( TERM.TERMInv ) );
			}

			// Search for duplicate - re-increment if duplicate.    
			// Set up common selection record JSREC for journal search  
			JournalLock ();

			// Set up selection data in common record JSREC         
			SetDefaultSelection ();

			// Status bits set off 
			JSREC.JSSTATOFF = TS1_VOID + TS1_REV;

			BfAscii( ( char * ) JSREC.JSINV, invoice_num,
					 sizeof( invoice_num ) );

			// Find first record that has this invoice number   
			// InvJrnIdent will be set by JournalSelFirst     
			InvJrnIdent = JournalSelFirst ();
			if ( 0x0000 == InvJrnIdent )
			{
				// Duplicate INVOICE was not found 
				loop_flag = False;
				retval = False;
			}
			JournalRelease ();
		}
		memcpy( TRINP.TRINV, JSREC.JSINV, S_TRINV );
	}
	return ( retval );
}


//-----------------------------------------------------------------------------
//  InDupCheck      If option set check for duplicate trans in batch.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Bool    True  - Duplicate found and rejected,
//                          False - otherwise.
//
//  Notes:          If duplicate found, prompts to user to accept.
//
//-----------------------------------------------------------------------------

Bool InDupCheck(  )
{
	Bool retval = False;
	UBYTE returnval;
    UWORD DupChkJrnIdent;

	// If duplicate transaction checking is enabled then search     
	// the journal for a matching total amount.                     
	if ( TCONF.TCOPT4 & TC4_DUPL )
	{
		// Set up common selection record JSREC for journal search  
		JournalLock ();

		// Set up selection data in common record JSREC         
		SetDefaultSelection ();

		// Status bits set off 
		JSREC.JSSTATOFF = TS1_VOID + TS1_REV + TS1_SETL;

		memcpy( JSREC.JSTOTAM, TRINP.TRTOTAM, S_TRTOTAM );

		// Find first record that has this same amount 
		DupChkJrnIdent = JournalSelFirst ();
		if ( DupChkJrnIdent != 0x0000 )
		{
			// Duplicate amount was found - check for duplicate PAN 
			while ( DupChkJrnIdent != 0x0000 )
			{
				// Read the record then test for matching PAN  
				JournalRead ( DupChkJrnIdent, &JRREC );

				// Does PAN and transaction type match  
				if ( !( memcmp( TRINP.TRPAN, JRREC.TRPAN, S_TRPAN ) ) &&
					 ( TRINP.TRKEY == JRREC.TRKEY ) )
				{
					// Duplicate amount and PAN and trans. type was found, prompt user  
					// Return false if user wants to use anyway.        
					returnval = YesNoEntry( &InDupYNEntry );
					if ( returnval != ENTER_KY )
					{
						retval = True;
					}
					else
					{
						retval = False;
					}
					break;
				}

				// Find the next  record that has this same amount 
				DupChkJrnIdent = JournalSelNext ( DupChkJrnIdent );
			}					// End of while 
		}
		JournalRelease ();
	}
	return ( retval );

}


//-----------------------------------------------------------------------------
//  InShift         Set up shift number if shift enabled.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

void InShift( void )
{
	
}


//-----------------------------------------------------------------------------
//  ReadData        Get card or keypad data.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Bool    True  - data read,
//                          False - canceled, timeout or invalid input.
//
//  Notes:          Cross-callable like ReadDataPrompt,
//                  but prompt is always SwipeCard.
//
//-----------------------------------------------------------------------------

Bool ReadData( void )
{   
    if ( TRINP.TRKEY == MAIL_ORDER )
      return ReadCardData ( EnterCard );
    else if ( TRINP.TRSTATUS[1] & TS2_FALLBACK || TRINP.TRSTATUS[1] & TS2_LOYALTYFBCK ) 
      return ReadCardData ( Swipecard );
    else
      return ReadCardData ( Swipecard );        
}


//-----------------------------------------------------------------------------
//  ReadDataPrompt  Get card or keypad data.
//
//  Parameters:     enum msg_id Mid Message ID of prompt
//
//  Global Inputs:  None.
//
//  Returns:        Bool            True  - data read,
//                                  False - canceled, timeout or invalid input.
//
//  Notes:          Cross-callable.
//
//-----------------------------------------------------------------------------

Bool ReadDataPrompt( enum msg_id Mid )
{
	return ReadCardData ( Mid );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  IncTermInv      Increment Terminal Invoice Number.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void IncTermInv( void )
{
	do
	{
		// Increment BCD number but skip over 000000 
		IncBcdLen( (UBYTE *)TERM.TERMInv, 3 );
	}
	while ( NullComp( ( char * ) TERM.TERMInv, 3 ) != False );

	// Set table LRC 
	SetTERMTabLRC(  );
}
