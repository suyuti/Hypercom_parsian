
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
//!	\brief
//!     Cross callable function to settle.
//!
//!	\param
//!     None
//!
//!	\return
//!	    None
//!
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

	// If there are no void sales or void refunds and no sales  
	// or refunds then do not continue.  
	if ( ( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTDBVCNT, S_TOTDBVCNT ) )
	 	&&
	 	( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTCRVCNT, S_TOTCRVCNT ) )
	 	&&
	 	( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTDBCNT, S_TOTDBCNT ) )
	 	&&
	 	( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTCRCNT, S_TOTCRCNT ) ) )
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
//!	\brief
//!     Close Batch
//!
//!	\param
//!     None
//!
//!	\return
//!	    None
//!
//-----------------------------------------------------------------------------
extern void ClBatch( void )
{
	UBYTE retval;
	char cnt;
	UBYTE i;
	char local[13];
	struct batch_rec *pBatchRec;
	int n;

	// Check if Acquirer known
	if ( 0 != TRINP.TRAQID )
	{
		// If terminal config option confirm batch totals is off    
		// then prompt for sales total and refund total amounts.    
		if ( !( TCONF.TCOPT1 & TC1_RECON ) )
		{
			// Display the transaction name and get the sales amount. 
			if ( !AmountEntry( &SaleAmtEntry, 1, 1 ) )
			{
				return;
			}
			else
			{
				// memcmpe the entered number, in Dspbuf, with the  
				// calculated number. If different then exit.       
				memset( ( UBYTE * ) local, '0', 12 );
				local[12] = 0;
				cnt = StrLn( Dspbuf, 12 );
				if ( cnt )
				{
					memcpy( &local[12 - cnt], Dspbuf, cnt );
				}
				AscHex( TRINP.TRSTOT, local, S_TRSTOT );
				if ( memcmp( TRINP.TRSTOT, TERMTOTALS.TOTBASIC.TOTDBAMT, 
							 S_TRSTOT ) )
				{
					pBatchRec = FindBatTab( TRINP.TRAQID );

					// See if pending set 
					if ( pBatchRec->BATFLAG & BT_PEND )
					{
						// Display adjust not allowed/sales totals wrong. 
						ShowInfoMsg( SalesTotalsWrong, AdjustNotAllowed );
						SDK_Beeper( TENMS * 40 );
						SDK_Wait( ONESECOND * 2 );
					}
					else
					{
						// Display sales totals wrong messages. 
						ShowErrMsg( SalesTotalsWrong );
					}
					return;
				}

				// Display the transaction name and get the refund amount. 
				if ( !AmountEntry( &RefundAmtEntry, 1, 1 ) )
				{
					return;
				}
				else
				{
					// memcmpe the entered number, in Dspbuf, with the  
					// calculated number. If different then exit.       
					memset( ( UBYTE * ) local, '0', 12 );
					local[12] = 0;
					cnt = StrLn( Dspbuf, 12 );

					if ( cnt )
					{
						memcpy( &local[12 - cnt], Dspbuf, cnt );
					}

					AscHex( TRINP.TRCTOT, local, S_TRCTOT );

					if ( memcmp( TRINP.TRCTOT, TERMTOTALS.TOTBASIC.TOTCRAMT,
								 S_TRCTOT ) )
					{
						pBatchRec = FindBatTab( TRINP.TRAQID );

						// See if pending set 
						if ( pBatchRec->BATFLAG & BT_PEND )
						{
							// Display adjust not allowed/refund totals wrong. 
							ShowInfoMsg( RefundTotalsWrong, AdjustNotAllowed );
							SDK_Beeper( TENMS * 40 );
							SDK_Wait( ONESECOND * 2 );
						}
						else
						{
							// Display sales totals wrong messages. 
							ShowErrMsg( RefundTotalsWrong );
						}
						return;
					}

				}
			}
		}
		else
		{
			// Preset custom display buffer to spaces. 
			// Calculate maximum number of characters to display on a line, in screen_limit 
			MaxCharDisp ();
			memset( CSTMSG, ' ',( UWORD ) screen_limit );
			CSTMSG[screen_limit] = '\0';
			n = TITLE_F_W;

			if ( n > 160 )
			{
				CSTMSG[screen_limit / 2] = '\0';
			}

			// Display sales total on upper line and yes/no on lower.  
			CvtAmt( ( char * ) &CSTMSG[( screen_limit - 13 )],
                    TERMTOTALS.TOTBASIC.TOTDBAMT );

			if ( n > 160 )
			{
				// Display sales total on upper line and yes/no on lower.  
				CvtAmt( ( char * ) &CSTMSG[( screen_limit / 2 - 13 )],
                        TERMTOTALS.TOTBASIC.TOTDBAMT );
			}

			// Move in "SALES TOTAL" without overlaying amount. 
			GetMsg( SalesTotal, Dspbuf );

			for ( i = 0; i < StrLn( Dspbuf, sizeof( Dspbuf ) ); i++ )
			{
				if ( ' ' == CSTMSG[i] )
				{
					CSTMSG[i] = Dspbuf[i];
				}
				else
				{
					break;
				}
			}

			// Prompt for YES or NO Keys 
			YNEntry.TitleMid = N_NullStr;
			YNEntry.Line1Mid = CustomMsg;
			YNEntry.Line2Mid = CorrectYesNo;

			retval = YesNoEntry( &YNEntry );

			// If yes then display refund total else exit. 
			if ( ENTER_KY == retval )
			{
				// Preset custom display buffer to spaces. 
				memset( CSTMSG, ' ', ( UWORD ) screen_limit );
				CSTMSG[screen_limit] = '\0';
				if ( n > 160 )
				{
					CSTMSG[screen_limit / 2] = '\0';
				}

				// Display refund total on upper line. 
				CvtAmt( ( char * ) &CSTMSG[( screen_limit - 13 )],
						TERMTOTALS.TOTBASIC.TOTCRAMT );

				if ( n > 160 )
				{
					// Display refund total on upper line. 
					CvtAmt( ( char * ) &CSTMSG[( screen_limit / 2 - 13 )],
							TERMTOTALS.TOTBASIC.TOTCRAMT );
				}

				// Move in "REFUND TOTAL" without overlaying amount.    
				GetMsg( RefundTotal, Dspbuf );
				for ( i = 0; i < StrLn( Dspbuf, sizeof( Dspbuf ) ); i++ )
				{
					if ( ' ' == CSTMSG[i] )
					{
						CSTMSG[i] = Dspbuf[i];
					}
					else
					{
						break;
					}
				}

				YNEntry.TitleMid = N_NullStr;
				YNEntry.Line1Mid = CustomMsg;
				YNEntry.Line2Mid = CorrectYesNo;

				// Prompt for YES or NO Keys 
				retval = YesNoEntry( &YNEntry );

				if ( ENTER_KY != retval )
				{
					return;
				}
			}
			else
			{
				// Exit if NO or cancel. 
				return;
			}
		}	// if ( !( TCONF.TCOPT1 & TC1_RECON ) )
	}

	CloseTerm ();
}


//-----------------------------------------------------------------------------
//!	\brief
//!     Close Batch
//!
//!	\param
//!     None
//!
//!	\return
//!	    None
//!
//-----------------------------------------------------------------------------
extern void ClBatcha( void )
{
	// Do not auto settle if the terminal has any open tabs. 
	if ( ChkOpenTabs(  ) )
		return;

	CloseTerm ();
}


//-----------------------------------------------------------------------------
//!	\brief
//!     ClBatche
//!
//!	\param
//!     None
//!
//!	\return
//! 	None
//!
//!	\note
//!     This function is a place holder only, but neccesary.
//!
//-----------------------------------------------------------------------------
extern void ClBatche( void )
{
}


//-----------------------------------------------------------------------------
//!	\brief
//!     RUpload
//!
//!	\param
//!     None
//!
//!	\return
//! 	None
//!
//!	\note
//!     This function is a place holder only, but neccesary.
//!
//-----------------------------------------------------------------------------
extern void RUpload( void )
{
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!	\brief
//!     Check Open Tabs
//!
//!	\param
//!     None
//!
//!	\return
//!	    Bool	    True  - any open tabs,
//!					False - otherwise.
//!
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
