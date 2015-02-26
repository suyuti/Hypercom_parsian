
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             reports.c
//      Function 79 - Reports Module
//
//=============================================================================
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
#include "msg.h"
#include "util.h"
#include "chkprinter.h"
#include "function.h"
#include "menu.h"
#include "hstprint.h"
#include "reports.h"
#include "perfutil.h"
#include "hstprint2.h"
#include "inputspw.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static void CashierReport( void );
static void IssuerTotals( void );
static UBYTE EnterCashier( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static const ENTRY_STR ShiftNumEntry = {
	N_Shift,					// Title 
	SelectShiftNum,				// Prompt 
	ZeroForAllShift,			// 2nd Prompt 
};		 

static const ENTRY_STR CashierNumEntry = {
	ReportMenuCashier,			// Title 
	CashierNum,					// Prompt 
	PressEnterForAll,			// Second line 
};

static const ENTRY_STR ServerNumEntry = {
	ReportMenuServer,			// Title 
	ServerNum,					// Prompt 
	PressEnterForAll,			// Second line 
};

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!	\brief
//!     Reports - Function 79
//!
//!	\param
//!     None
//!
//!	\return
//!	    None
//!
//-----------------------------------------------------------------------------
extern void Reports( void )
{
	UBYTE Status;
	char temp[2];

	// See if printer is available 
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// No; exit 
		return;
	}

	// Is Shift processing enabled? 
	if ( TCONF.TCOPT5 & TC5_SHIFT )
	{
		// Prompt for Shift Number (2 Digits - BCD) 
		Status = NumEntry( 1, 2, 1, &ShiftNumEntry );

		// See if Cancel pressed 
		if ( !Status )
		{
			// Yes; exit 
			return;
		}

		// Save the Shift Number entered - BCD number 99 or less 
		if ( Dspbuf[1] == '\0' )
		{
			temp[0] = '0';
			temp[1] = Dspbuf[0];
		}
		else
		{
			temp[0] = Dspbuf[0];
			temp[1] = Dspbuf[1];
		}

		AscHex( ( UBYTE * ) & TRINP.TRSHIFT, temp, ( UBYTE ) 1 );
	}

	// Get Supervisor Password with Local Option check 
	if ( !InputSPswd( N_NullStr, TCL_NO_RPPW ) )
	{
		// No; exit 
		return;
	}

	DrawMenu( MENU_REPORTS );

	// Call the selected Option Processor 
	do
	{
		Status = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

		switch ( Status )
		{
			case 'A':			// Till Report 

				// Set next event to PRTTILL (Till Report) 
				CurrentEvent.NEXTEVENT = PRTTILL;
				break;

			case 'B':			// Cashier Report 
				CashierReport(  );

				// Pass cancel status from CashierReport forward. 
				if ( CANCEL_KY == ButtonId )
					Status = CANCEL_KY;
				else if ( B_PREVIOUS_NEW_MENU == ButtonId )
					DrawMenu( MENU_PREVIOUS );	// draw the reports menu again 
				break;

			case 'C':			// Audit Trail 
				// Set next event to PRTLIST (Audit Log Report) 
				CurrentEvent.NEXTEVENT = PRTLIST;
				// since we won't be coming back here, need to clear menu stack 
				ClearMenus ();
				break;

			case 'D':			// Issuer Totals 
				IssuerTotals(  );
				DrawMenu( MENU_REDRAW );
				break;

			case 'E':			// Open Tabs Report 
				// Notify user -- "PRINTING, WAIT..." 
				ShowInfoMsg( PrintingWait, PleaseWait );

				PrintOpenTabs ();
				DrawMenu( MENU_REDRAW );
				break;

			case 'F':			// Reprint 
				Reprint ();
				if ( CANCEL_KY == ButtonId )
					Status = CANCEL_KY;
				else if ( B_PREVIOUS_NEW_MENU == ButtonId )
					DrawMenu( MENU_PREVIOUS );	// draw the reports menu again 
				break;
            case 'G':			// Performance Report

		        // Notify user -- "PRINTING, WAIT..." 
			    ShowInfoMsg(PrintingWait, PleaseWait);

			    PrintPerformRpt ();
				DrawMenu(MENU_REDRAW);
			    break;

			  case 'H':			// Config Report
				// Set next event to Configuration report 
			    DoConfigRpt ();
				Status = CANCEL_KY;
				break;

			case B_PREVIOUS:	// Go back one menu level 
				DrawMenu( MENU_PREVIOUS );
				break;

			case B_PREVIOUS_NEW_MENU:	// Go back one menu level 
				// ButtonId from input will cause back one level.
				break;

			case B_NEXT:
				DrawMenu( MENU_NEXT);
				break;

			case CLEAR_KY:		// Clear was pressed so exit.   
				break;

			default:
				break;
		}
		// Only contine if printer is still available 
        if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
		{
			// No; exit 
			Status = 0;
		}

	}
	while ( Status && ( Status != CANCEL_KY ) && !( CurrentEvent.NEXTEVENT )
			&& ( Status != B_PREVIOUS_NEW_MENU ) && ( Status != CLEAR_KY ) );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!	\brief
//!     Cashier Report - Function 79 Option 2
//!
//!	\param
//!     None
//!
//!	\return
//! 	None
//!
//-----------------------------------------------------------------------------
static void CashierReport( void )
{
	UBYTE Option;
	// Call the selected Option Processor 
	DrawMenu( MENU_CASHIER_REPORT );

	do
	{
		Option = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

		// Get the Cashier Number allowing NULL entry 
		if ( ( Option >= 'A' ) && ( Option <= 'C' ) )
		{
			if ( EnterCashier(  ) )
			{
				// Okay; Cashier/Server Menu options 1-3 translate to Report
				// Options 0-2 
				RPTOPTS = Option - 'A';

				// Notify user -- "PRINTING, WAIT..." 
				ShowInfoMsg( PrintingWait, PleaseWait );

				// Print Cashier Report 
				PrintCashierAudit ();

				DrawMenu( MENU_REDRAW );
			}
			else
			{
				// Clear or timeout from enter cashier screen   
				// will cancel all the way back to idle.        
				Option = CANCEL_KY;
			}
		}
		else if ( Option == B_PREVIOUS )
		{
			DrawMenu( MENU_PREVIOUS );
		}
		else if ( Option == B_NEXT )
		{
			DrawMenu( MENU_NEXT );
		}
	}
	while ( Option && ( Option != CANCEL_KY ) && ( Option != CLEAR_KY )
			&& ( Option != B_PREVIOUS_NEW_MENU ) );

	if ( !Option || ( CANCEL_KY == Option ) || ( CLEAR_KY == Option ) )	// Need to go back to idle 
		ButtonId = CANCEL_KY;

	Titlebuf[0] = 0;
}


//-----------------------------------------------------------------------------
//!	\brief
//!     Issuer Totals - Function 79 Option 4
//!
//!	\param
//!     None
//!
//!	\return
//!     None
//!
//-----------------------------------------------------------------------------
static void IssuerTotals( void )
{
	// Notify user -- "PRINTING, WAIT..." 
	ShowInfoMsg( PrintingWait, PleaseWait );

	// Do Issuer Receipt 
	IssuerReceipt ();
}


//-----------------------------------------------------------------------------
//!	\brief
//!     Enter Cashier Number
//!
//!	\param
//!     None
//!
//!	\return
//!	    Bool	    True  - valid Cashier Number entered or not needed
//!					False - Cashier Number entry canceled.
//!
//-----------------------------------------------------------------------------
static UBYTE EnterCashier( void )
{
	UBYTE Status;
	UBYTE i;

	// Set status to true 
	Status = True;

	// See if Cashier Feature is on 
	if ( TCONF.TCOPT1 & TC1_CASHIER )
	{
		// Get the Cashier/Server Number with zero minimum length 
		if ( TCONF.TCOPT4 & TC4_SRV_PRM )
			Status = NumEntry( 1, 4, 0, &ServerNumEntry );
		else
			Status = NumEntry( 1, 4, 0, &CashierNumEntry );

		// If not cancelled 
		if ( Status )
		{
			// Okay; see if NULL entry 
			if ( Dspbuf[0] )
			{
				// No; need to pad the Cashier Number with trailing 
				// spaces if the length is less than S_TRCASHIER 
				for ( i = 0; i < S_TRCASHIER; i++ )
				{
					// See if character is a null 
					if ( !Dspbuf[i] )
					{
						// Yes; replace with a space 
						Dspbuf[i] = ' ';
					}
				}

				// Save the entered Cashier Number 
				memcpy( TRINP.TRCASHIER, Dspbuf, S_TRCASHIER );
			}
			else
			{
				// Clear the cashier information in case we return      
				// to the menu screens after cashier reports.
				memset( TRINP.TRCASHIER, 0,S_TRCASHIER );
			}
		}
	}

	// Return the status 
	return ( Status );
}
