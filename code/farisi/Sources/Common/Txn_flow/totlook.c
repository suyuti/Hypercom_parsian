
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             totlook.c
//      Look at the Totals Routines.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"

#include "utllib.h"
#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "util.h"
#include "sdkgui.h"
#include "tables.h"
#include "totlook.h"
#include "totals.h"
#include "journal.h"
#include "cvtamt.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static void TotalLookDisp( void );
static Bool GetNextIssuer( void );
static Bool IncIssuer( void );
static void DecIssuer( void );
static Bool DoNoMoreTotals( void );
static void BuildDisplay( void );
static void BuildGrandTotal( void );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static UBYTE IsIndex;			// Issuer Index              
static UBYTE IsCount;			// Issuer Count              
static UBYTE SavIsIndex;		// Saved Issuer Index        
static UBYTE SavIsCount;		// Saved Issuer Count        
static UBYTE Direction;			// Scroll Direction          
static Bool TotalsFound;		// Batch Totals were found   
static Bool DoGrandTot;			// Do Grand Totals here      


// Journal Select Default 
static const struct jrnfind_rec JSDEF = {
	0x0000,						// Ident              
	0xff,						// Transaction Key    
	0x00,						// All Acquirers      
	0x00,						// All Issuers        
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	// All Invoices       
	0x00,						// Status Bits On     
	0x00,						// Status Bits 2 On   
	TS1_REV + TS1_SETL,			// Status Bits Off    
	0x00,						// Status Bits 2 Off  
	0xff,						// All Records        
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	// All Amounts        
	0x00,						// Acquirer to Ignore 
	{0x00, 0x00, 0x00, 0x00},	// All Cashiers       
	0x00						// Shift Number       
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Look at the Totals.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------

extern void TotalLook( void )
{
	UBYTE Key;

	// Show Grand Totals first 
	DoGrandTot = True;

	// First Issuer 
	IsIndex = 0;

	// Times to loop 
	IsCount = ISMAX;

	// Direction Forward 
	Direction = 0x00;

	// No Totals Found yet 
	TotalsFound = False;

	// Loop till Done 
	do
	{
		// Show Grand Totals? 
		if ( DoGrandTot )
		{
			// Yes; build the Grand Totals 
			BuildGrandTotal(  );

			// See if Grand Totals Exist 
			if ( !NullComp( ( char * ) &TERMTOTALS, TOTSZE ) )
			{
				// Yes; display them 
				TotalLookDisp(  );
			}
			else
			{
				// No; see if any totals found 
				if ( !DoNoMoreTotals(  ) )
				{
					// No; quit the loop 
					break;
				}
			}
		}
		else
		{
			// No; see if Table Populated 
            if ( 0x00 != ISTAB[IsIndex].ISNUM )
			{
				// Yes; lock the Journal 
				JournalLock ();

				// Use Default Search 
				memcpy( ( UBYTE * ) & JSREC, ( UBYTE * ) & JSDEF, JSSZE );

				// Get Issuer Id 
                JSREC.JSISID = ISTAB[IsIndex].ISID;

				// Calculate Totals for this Issuer 
				// Calculate Totals for all Issuers 
				CalcTotals_Jsrec ();

				// Release Journal 
				JournalRelease ();

				// See if Totals Exist 
				if ( !NullComp( ( char * ) &TERMTOTALS, TOTSZE ) )
				{
					// Yes; display them 
					TotalLookDisp(  );
				}
				else
				{
					// No; get the next issuer and check for totals 
					if ( !GetNextIssuer(  ) )
					{
						// No totals; quit the loop 
						break;
					}
					else
					{
						// Totals; continue with the next issuer 
						continue;
					}
				}
			}
			else
			{
				// No; get the next issuer and check for totals 
				if ( !GetNextIssuer(  ) )
				{
					// No totals; quit the loop 
					break;
				}
				else
				{
					// Totals; continue with the next issuer 
					continue;
				}
			}
		}

		// Do till done 
		do
		{
			// Draw the Screen 
			DrawScreen( RTOTALS_SCR );

			// Get a key from the user 
			Key = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
			// Check if cancel or Time-Out 
			if ( ( CANCEL_KY == Key ) || ( 0 == Key ) )
			{
				// Yes; quit the loop 
				break;
			}

			// No; base action upon the key press 
			switch ( Key )
			{
				case B_NEXT:	// increment issuer index and check for any totals 
					if ( !IncIssuer(  ) )
					{
						// No totals; set key to cancel 
						Key = 0;
					}
					break;

				case B_PREVIOUS:	// decrement issuer index 
					DecIssuer(  );
					break;

				default:		// Invalid key pressed 
					Key = 1;
					break;
			}
		}
		while ( 1 == Key );

		// Check for User cancel or no totals 
		if ( 0 == Key )
		{
			// Yes; quit the main loop 
			break;
		}
	}
	while ( Key != CANCEL_KY );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Display Totals.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void TotalLookDisp( void )
{
	// Signal that some Totals were found 
	TotalsFound = True;

	// Build Display 
	BuildDisplay(  );

	// Display Review Totals Screen 
	DrawScreen( RTOTALS_SCR );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Get the next issuer based upon direction.
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - okay to continue,
//!                 False - no Totals have been found.
//!
//!  \note
//!     Globals:  Direction.
//!
//-----------------------------------------------------------------------------
static Bool GetNextIssuer( void )
{
	Bool retval;

	// See if Forward Direction 
	if ( 0x00 == Direction )
	{
		// Yes; search Forward 
		retval = IncIssuer(  );
	}
	else
	{
		// No; search Backward 
		DecIssuer(  );

		// Return value is okay 
		retval = True;
	}

	// Return the value 
	return ( retval );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Increment Issuer Index.
//!
//!  \param
//!     None.
//!
//!  \return
//!     Bool        True  - okay to continue,
//!                 False - no Totals have been found.
//!
//-----------------------------------------------------------------------------
static Bool IncIssuer( void )
{
	// Look for Issuer Count = 0 
	if ( 0x00 == IsCount )
	{
		// Yes; no more Totals, check if any Totals found 
		if ( !DoNoMoreTotals(  ) )
		{
			// No; return Totals have not been found 
			return ( False );
		}
	}
	else
	{
		// No; see if Grand Totals wanted 
		if ( DoGrandTot )
		{
			// Yes; don't look at Grand Totals 
			DoGrandTot = False;
		}
		else
		{
			// No; increment Issuer Index 
			IsIndex++;

			// Decrement Issuer Count 
			IsCount--;

			// Indicate Forward direction 
			Direction = 0x00;
		}
	}

	// Return okay to continue 
	return ( True );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Decrement Issuer Index.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void DecIssuer( void )
{
	// Look for end of Issuer Table 
	if ( ISMAX == IsCount )
	{
		// Yes; show Grand Totals 
		DoGrandTot = True;

		ShowInfoMsg( NoMoreTotals, BatchTotals );
		SDK_Wait( TENMS * 100 );

	}
	else
	{
		// No; decrement Issuer Index 
		IsIndex--;

		// Increment Issuer Count 
		IsCount++;

		// Indicate Backward direction 
		Direction = 0x01;
	}
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Indicates if more totals.
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - Totals have been found,
//!                 False - otherwise.
//!
//!  \note
//!     Globals:  TotalsFound.
//!
//-----------------------------------------------------------------------------
static Bool DoNoMoreTotals( void )
{
	// Have any Totals been found 
	if ( TotalsFound )
	{
		// Yes; indicate a change of direction 
		Direction = ~Direction;

		// Restore Last Record Displayed 

		// Restore Issuer Index 
		IsIndex = SavIsIndex;

		// Restore Issurer Count 
		IsCount = SavIsCount;

		// Notify user: "NO MORE TOTALS" 
		ShowInfoMsg( NoMoreTotals, BatchTotals );
		SDK_Wait( TENMS * 100 );

		// Indicate that Totals were found 
		return ( True );
	}
	else
	{
		// No; notify user: "NO BATCH TOTALS" 
		ShowInfoMsg( NoBatchTotals, BatchTotals );
		SDK_Wait( TENMS * 100 );

		// Indicate that no Totals were found 
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Build Totals Display.
//!
//!  \param
//!     None.
//!
//!  \return
//!     None
//!
//!  \note
//!     Globs: TERMTOTALS. Updates CSTMSG, CSTMSG2, CSTMSG3, CSTMSG4 and CSTMSG5.
//!
//-----------------------------------------------------------------------------
static void BuildDisplay( void )
{
	UBYTE length;

	int limit;
	CHARSIZE charsz;

	// Save Issuer Index of last displayed record 
	SavIsIndex = IsIndex;

	// Save Issuer Count of last displayed record 
	SavIsCount = IsCount;

	// Clear the CSTMSG buffer
	memset( (UBYTE *)CSTMSG, 0,sizeof( CSTMSG) );
	
	// Set up first text line, "TOTALS" or Issuer Name 
	if ( DoGrandTot )
	{
		// Yes; add "TOTALS" to the display buffer 
		GetMsg( BatchTotals, CSTMSG );
		CSTMSG[StrLn(CSTMSG, sizeof( CSTMSG))] = ' ';
	}
	else
	{
		// Add Issuer Name to the display buffer 
        MoveItIss( CSTMSG, &ISTAB[IsIndex].ISNAME, S_ISNAME );
		CSTMSG[S_ISNAME] = '\0';
	}

	// Buffer not used
	memset( (UBYTE *)CSTMSG2, 0,sizeof(CSTMSG2) );

	// Set the display buffer -- build the Information line here 
	memset( CSTMSG3, ' ',sizeof( CSTMSG3 ) );
	memset( CSTMSG4, ' ',sizeof( CSTMSG4 ) );
	memset( CSTMSG5, ' ',sizeof( CSTMSG5 ) );

	// Add the Debit Count 
	BfAscii( CSTMSG3, TERMTOTALS.TOTBASIC.TOTDBCNT, 2 );

	// Add the Credits Count 
	BfAscii( CSTMSG4, TERMTOTALS.TOTBASIC.TOTCRCNT, 2 );

	// Blank fill leading zeros (first two positions) 
	// See if zero in first position 
	if ( '0' == CSTMSG3[0] )
	{
		// Yes; blank it 
		CSTMSG3[0] = ' ';

		// See if zero in second position 
		if ( '0' == CSTMSG3[1] )
		{
			// Yes; blank it 
			CSTMSG3[1] = ' ';
		}
	}
	if ( '0' == CSTMSG4[0] )
	{
		// Yes; blank it 
		CSTMSG4[0] = ' ';

		// See if zero in second position 
		if ( '0' == CSTMSG4[1] )
		{
			// Yes; blank it 
			CSTMSG4[1] = ' ';
		}
	}

	// Convert the Debit Amount 
	CvtAmt( &CSTMSG3[8], TERMTOTALS.TOTBASIC.TOTDBAMT );

	// Calculate length of "SALES" that may be added 
	for ( length = 0; length < 6; length++ )
	{
		// See if data exits in an "SALES" spot 
		if ( ' ' != CSTMSG3[5 + length] )
		{
			// Yes; exit the loop 
			break;
		}
	}
	// Get the 'SALES' prompt.  Display what will not overwrite the amount field 
	GetMsg( Msg_Sales, Dspbuf );
	length = StrLn( Dspbuf, length );

	// Add what will fit of "SALES" 
	memcpy( &CSTMSG3[5], Dspbuf, length );

	// Convert the Credit Amount 
	CvtAmt( &CSTMSG4[8], TERMTOTALS.TOTBASIC.TOTCRAMT );

	// Calculate length of "REFUND" that may be added 
	for ( length = 0; length < 6; length++ )
	{
		// See if data exits in an "REFUND" spot 
		if ( ' ' != CSTMSG4[5 + length] )
		{
			// Yes; exit the loop 
			break;
		}
	}

	// Get the 'REFUND' prompt.  Display what will not overwrite the amount field 
	GetMsg( RefundTran, Dspbuf );
	length = StrLn( Dspbuf, length );

	// Add what will fit of "REFUND" 
	memcpy( &CSTMSG4[5], Dspbuf, length );

	// Get Font size 
	SDK_GuiGetCharSize( &charsz );

	// Maximum characters for the Dialogue line
	limit = ( SCR_PIXEL_WIDTH - ( TEXT_INDENT * 2 ) ) / ( charsz.Width );

	// Null terminate the first display lines 
	CSTMSG3[limit] = '\0';
	CSTMSG4[limit] = '\0';
	CSTMSG5[limit] = '\0';

}


//-----------------------------------------------------------------------------
//!  \brief
//!     Build Grand Totals
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void BuildGrandTotal( void )
{
	// Lock the Journal 
	JournalLock ();

	// Use Default Search 
	memcpy( ( UBYTE * ) & JSREC, ( UBYTE * ) & JSDEF, JSSZE );

	// Calculate Totals for all Issuers 
	CalcTotals_Jsrec ();

	// Release Journal 
	JournalRelease ();
}


