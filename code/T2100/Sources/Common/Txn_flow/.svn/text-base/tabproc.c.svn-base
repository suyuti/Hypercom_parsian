
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             tabproc.c
//      Main functions to handle OPEN TAB transaction.
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
#include "tables.h"
#include "showtrk2.h"
#include "findtran.h"
#include "receipt.h"
#include "manuexp.h"
#include "amtproc.h"
#include "app1.h"
#include "flaghost.h"
#include "respdsp.h"
#include "predial.h"
#include "acctproc.h"
#include "hstprint2.h"
#include "validexp.h"

#include "tabproc.h"

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

static const ENTRY_STR TabMenu = {
	N_Tab,						// Title 
	TabProcMsg1,				// Prompt 
	TabProcMsg2,				// Default value 
};

static const ENTRY_STR CardNumEntry = {
	N_NullStr,					// Title 
	EnterCard,					// Prompt 
	N_NullStr,					// Default value is in INJECTKEY 
};

static ENTRY_STR YNEntry;

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  TabProcessing   Main function for processing the Open Tab feature.
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

extern void TabProcessing( void )
{
  ShowErrMsg( N_FuncNotSupported );
}


//-----------------------------------------------------------------------------
//  OpenTabProc     Open Tab transaction routine.
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

extern void OpenTabProc( void )
{
	UBYTE retval;


	// Get the transaction descriptive text and options 
	SecondEvent.TRNKEY = OPENTAB;
	FindTran ();
	memcpy( &CurrentEvent, &SecondEvent, sizeof( struct event_entry ) );

	TRINP.TRKEY = OPENTAB;

	// Use common routine to get account data from card or kbd. 
	if ( !ReadData(  ) )
		return;

	// validate account number - status stored in global chRetval 
	if ( !AcctProc () )
		return;

	do
	{

		// SALE initiated by a key press? If so, get EXP date 
		if ( ( INJECTKEY ) && ( (TRINP.TRISPTR->ISOPT1) & IS1_EXPD ) )
		{
			if ( !ManualExp () )	// CANCEL key pressed? 
				return;
		}

		if ( ( (TRINP.TRISPTR->ISOPT2) & IS2_CH_EXPD ) )
			retval = ValidateEXP(  );
		else
			retval = 1;

		if ( !retval )
		{
			ShowErrMsg( ExpiredCard );

			if ( INJECTKEY )
				ShowIssuer_Tran(  );
			else
				return;
		}
		else					// For Manual entry, we need to check the IMPRINT OPTION 
		if ( INJECTKEY )
		{
			if ( TCONF.TCOPT3 & TC3_IMPRINT )
			{
				memset( ( UBYTE * ) CSTMSG2, 0,sizeof( CSTMSG2 ) );
				GetMsg( TakeImprint, CSTMSG2 );
				YNEntry.TitleMid = N_NullStr;
				YNEntry.Line1Mid = CustomMsg2;
				YNEntry.Line2Mid = CorrectYesNo;
				retval = YesNoEntry( &YNEntry );
				if ( retval != ENTER_KY )
					return;
			}
		}
	}
	while ( !retval );

	// If Card Swipe, determine whether we should display track 2 data 
	if ( !INJECTKEY )
	{
		
		if ( (TRINP.TRISPTR->ISOPT4) & IS4_PANLAST4 )
		{
			retval = GetPAN4(  );
			if ( !retval )
			{
				ShowErrMsg( InvalidCard );
				return;
			}
			else if ( retval == CANCEL_KY )
				return;
			else
				ShowIssuer_Tran(  );
		}
	}

	// Test for journal full condition  
	if ( JournalFullTest(  ) )
		return;

	// Get shift number if shift processing is enabled. 
	InShift(  );

	// Get Cashier Number; allow NULL entry 
	
	// Perform a Pre-Dial if allowed 
	PreDial ();

	// Accept amount entry - status stored in global chRetval 
	if ( !AmtProc () )
		return;

	if ( InInvoiceNum(  ) )
		return;

	if ( InDupCheck(  ) )
		return;

	// Set flags to allow user cancel and do not preprint.  
	FlagHostCntl = FLAG_ALLOW_CANCEL;

	// Flag the host process  
	FlagHost ();

	// Save host response text in terminal flag table.  
	SaveResponseText ();

	// Display Response 
	RespDisplay(  );

	//=========================================================================
	// PRINT RECEIPT                                                        
	//=========================================================================

	Receipt ();
}


//=============================================================================
// Private function definitions
//=============================================================================
