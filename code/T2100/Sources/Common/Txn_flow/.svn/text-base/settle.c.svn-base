
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             settle.c
//      Settle/Close Batch User Interface routines.
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
#include "tables.h"
#include "chkprinter.h"
#include "totals.h"
#include "journal.h"
#include "disproc.h"
#include "close.h"
#include "cvtamt.h"
#include "amtentry.h"
#include "getaqid.h"
#include "chkbatch.h"

#include "settle.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static Bool ChkOpenTabs( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static ENTRY_STR YNEntry;

static const ENTRY_STR SaleAmtEntry =	// structure for AmountEntry() 
{
	N_NullStr,
	N_NullStr,
	SalesTotal,
};

static const ENTRY_STR RefundAmtEntry =	// structure for AmountEntry() 
{
	N_NullStr,
	N_NullStr,
	RefundTotal,
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//	Settle			Cross callable function to settle.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

extern void Settle( void )
{
	UBYTE status;
	char num_hosts, flag_closebatch;

	/* Setup Title for Supervisor Card prompt */
	GetMsg( SettleTran, DefaultTitle );
	

	TRINP.TRKEY = SETTLE;

	num_hosts = AqidCount(  );	// count the number of acquirers 

	// we don't have any acquirer to settle, just return 
	if ( num_hosts == 0 )
		return;

	// if we have only one acquirer in acquirer table, don't need to pop up 
	// display the host settle menu. Just set the proper data to TRINP      
	if ( num_hosts == 1 )
	{
   		TRINP.TRAQID  = AQTAB[0].AQID;
                TRINP.TRAQPTR = &AQTAB[0];
	}
	else
	{
		// Multiple hosts. Test for 'no host select' option. If this option 
		// is set then do not prompt for acquirers selection                
		if ( TCONF.TCOPT5 & TC5_NOHSPRM )
		{
			// Be sure acquired id is set to zero for multi host settle 
			TRINP.TRAQID = 0;
		}
		else if ( TCONF.TCOPT5 & TC5_HSMENU )
		{
			DrawMenu( MENU_HOST_BATCH_SETTLE );

			do					// Call the selected option processor 
			{
				status = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );
				if ( status == B_PREVIOUS )		// Go back one menu level 
				{
					DrawMenu( MENU_PREVIOUS );
				}
				else if ( status == B_NEXT )	// Go forward one menu level 
				{
					DrawMenu( MENU_NEXT );
				}
				else if ( 'Z' == status )		// settle all hosts 
				{
					// set to zero for multi host settle 
					TRINP.TRAQID = 0;
					DrawMenu( MENU_REDRAW );
					break;
				}
				else if ( ( status >= 'A' ) && ( status <= 'A' + 7 ) )
				{
        	    	TRINP.TRAQID  = AQTAB[status - 'A'].AQID;
            		TRINP.TRAQPTR = &AQTAB[status - 'A'];
					DrawMenu( MENU_REDRAW );
					break;
				}

			}
			while (	   status 
					&& ( CANCEL_KY != status ) 
					&& ( CLEAR_KY != status ) 
					&& ( B_PREVIOUS_NEW_MENU != status ) );

			if (   ( status == CANCEL_KY ) 
				|| ( CLEAR_KY == status ) 
				|| ( status == 0 ) )
			{
				ButtonId = CANCEL_KY;
				return;
			}

			if ( status == B_PREVIOUS_NEW_MENU )
			{
				ButtonId = status;
				return;
			}
		}
		else
		{
			// Prompt for the acquirer - TRAQID and TRAQPTR will be set if  
			// single acquirer or valid acquirer selected. If 0 is selected 
			// then the acquirer ID and pointer will be 0.                  
			if ( !GetAQID( True ) )
				return;
		}
	}
        
	// Do not settle if the terminal has any open tabs. 
	if ( ChkOpenTabs(  ) )
	{
		// Open tabs exist. Issue close all tabs message. 
		ShowErrMsg( CloseOpenTabs );
		return;
	}

	// Set batch type to 0 for close. 
	TRINP.TRBATTYP = 0;

	// Don't settle this host if close is already settled.  
	if ( CheckSettle( TRINP.TRAQID ) )
	{
		// Display message - no batch totals.  
		ShowErrMsg( NoBatchTotals );
		return;
	}

	CalcTotals ();

	// Is signature capture defined and enabled? 
	flag_closebatch = False;
	
	if ( ( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTCRCNT, S_TOTCRCNT  ) )
	 	&&
	 	( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTDBCNT, S_TOTDBCNT ) )
	 	&&
	 	( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTRFCNT, S_TOTRFCNT ) )
                &&
	 	( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTDBRFCNT, S_TOTDBRFCNT ) ) 
	 	&&
	 	( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTAUTCNT, S_TOTAUTCNT ) ) 
                &&
	 	( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTPAYCNT, S_TOTPAYCNT ) ) 
                &&
	 	( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTAUTRFCNT, S_TOTAUTRFCNT ) ) )
	{
		// Display message - no batch totals.  
		ShowErrMsg( NoBatchTotals );
		return;
	}

	// Change transaction key to CLBATCH. 
	TRINP.TRKEY = CLBATCH;

	// Put the Transaction Name in TRINP for later display 
	memcpy( TRINP.TRTRANBUF, CurrentEvent.TRNTEXT,
			sizeof( CurrentEvent.TRNTEXT ) );

	// Set next event for CLBATCH - send batch to the host. 
	CurrentEvent.NEXTEVENT = CLBATCH;
}


//-----------------------------------------------------------------------------
//	ClBatch
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

extern void ClBatch( void )
{
    CloseTerm ();
}


//-----------------------------------------------------------------------------
//	ClBatcha
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

extern void ClBatcha( void )
{
	// Do not auto settle if the terminal has any open tabs. 
	if ( ChkOpenTabs(  ) )
		return;

	CloseTerm ();
}


//-----------------------------------------------------------------------------
//	ClBatche
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			This function is a place holder only, but neccesary.
//
//-----------------------------------------------------------------------------

extern void ClBatche( void )
{
}


//-----------------------------------------------------------------------------
//	RUpload
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			This function is a place holder only, but neccesary.
//
//-----------------------------------------------------------------------------

extern void RUpload( void )
{
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//	ChkOpenTabs
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Bool	True  - any open tabs,
//							False - otherwise.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

static Bool ChkOpenTabs( void )
{
	// Get a lock on the journal before using JSREC.    
	JournalLock ();

	// Set up selection data in common record JSREC         
	SetDefaultSelection ();

	// Select OPENTAB transactions.  
	JSREC.JSKEY = OPENTAB;

	if ( 0x0000 == JournalSelFirst ( ) )
	{
            // Release the lock on the journal. 
            JournalRelease ();

            // No open tabs exist in the terminal.  
            return ( False );
	}
	else
	{
            // Release the lock on the journal. 
            JournalRelease ();
            // Open tabs do exist in the terminal.  
            return ( True );
	}
}

