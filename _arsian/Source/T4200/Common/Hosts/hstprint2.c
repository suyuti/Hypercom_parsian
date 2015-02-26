//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             hstprint2.c
//      Host2 receipt software module
//
//=============================================================================
#include "clib.h"

#include "basictyp.h"
#include "fixdata.h"
#include "sdk.h"
#include "sdktime.h"
#include "utllib.h"

#include "struct.h"
#include "comdata.h"
#include "appdata.h"
#include "msg.h"
#include "prtdef.h"
#include "util.h"
#include "tables.h"
#include "chkprinter.h"
#include "findtran.h"
#include "journal.h"
#include "hstprint.h"
#include "prtdrv.h"
#include "host.h"
#include "hstprint2.h"

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


//=============================================================================
// Public function from other modules
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Opens Print Tabs Report
//!
//! \return
//!     None
//!
//! \note
//!     As the global input TRINP.TRSHIFT contains shift number to use (0 = All).
//-----------------------------------------------------------------------------
extern void PrintOpenTabs( void )
{
    static UWORD JrnIdent;
    static struct trandata_rec TrnBuf;		// journal record

	// See if Printer is available
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller
		return;
	}

	// Initialize printer PCL data and flags
    PrintInit ();

	// Lock the Journal
	JournalLock ();

	// Set up Default Record
	SetDefaultSelection ();

	// Shift Number to use (0 = All)
	JSREC.JSSHIFT = TRINP.TRSHIFT;

	// Look for Open Tab Transactions only
	JSREC.JSKEY = OPENTAB;

	// See if we can find first record
	JrnIdent = JournalSelFirst ();

	// See if any entries in the Journal
	if ( 0x0000 == JrnIdent )
	{
		// No; Release Journal
		JournalRelease ();

		// Notify user: "NO TRANSACTIONS"
		ShowErrMsg( NoTransactions );

		// Return to caller
		return;
	}

	// Loop till done
	do
	{
		// Read the record that was found
		JournalRead ( JrnIdent, &TrnBuf );

		// Print the Transaction (Open Tabs)
        PrintDriver( RP_OPEN_TABS, &TrnBuf );

		// Select the next record
		JrnIdent = JournalSelNext ( JrnIdent );

	}
	while ( ( 0x0000 != JrnIdent ) &&
			( PrinterOK == PrinterErrorFlags ) );

	// Eject Page
    if( PrinterOK == PrinterErrorFlags )
	{
		SDK_PrintString( "\f" );
	}

	// Release Journal
	JournalRelease ();

	SDK_PrinterClose(  );

	// If printer error was reported then display error here.
	if ( PrinterOK != PrinterErrorFlags )
		DisplayPrintError ();

}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints out cashier signed-on report
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void Cashier_SignonRpt( void )
{
	int idx;
	Bool CashierFound = False;

	// Initialize printer PCL data and flags
    PrintInit ();

	for ( idx = 0; idx < CASHIER_MAX; idx++ )
	{
		if ( ( CASHIER_USED & CASHTAB[idx].CASHIER_FLAG ) )
		{
			memcpy( TRINP.TRCASHIER, CASHTAB[idx].CASHIER, S_TRCASHIER );
			memcpy( TRINP.TRTILLNO, CASHTAB[idx].TILLNO, S_TRTILLNO );

			PrintDriver ( RP_LOGON, &TRINP );

			PROPT |= OPT1MASK;	// Line feed every other cashier
			CashierFound = True;
		}
	}

	// Flush the print buffer with a form-feed but only if printing took place.
	if ( CashierFound )
	{
    	if( PrinterOK == PrinterErrorFlags )
		{
			SDK_PrintString( "\f" );
		}
	}
	else
	{
		// No cashiers.  Display appropriate error message.
		if ( TCONF.TCOPT4 & TC4_SRV_PRM )
		{
			// Use server message
			ShowErrMsg( Msg_SrvTabEmpty );
		}
		else
		{
			// Use cashier message
			ShowErrMsg( Msg_CshTabEmpty );
		}
	}

	// Close the Printer
	SDK_PrinterClose(  );

	// Final check for printer status.  Sign on reports is
	// short and out of paper may not show up during printing.
	SDK_Wait( ONESECOND * 1 );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints a settle receipt
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void SettleReceipt( void )
{
	// See if Printer is available
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller
		return;
	}

	// Initialize printer PCL data and flags
    PrintInit ();

    // Print it
	PrintDriver ( RP_SETTLE, &TRINP );

	SDK_PrinterClose(  );

	// If printer error was reported then display error here.
	if ( PrinterOK != PrinterErrorFlags )
		DisplayPrintError ();
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints Settlement Summary Report
//!
//! \param
//!		[in]ReportOptions		Report printing options
//!
//! \return
//!     0 on Printer error else 1
//-----------------------------------------------------------------------------
extern UBYTE PrintSettleSummary( UBYTE ReportOptions )
{
	UBYTE chRetval;

	// See if Printer is available
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller
		return chRetval;
	}

	// Initialize printer PCL data and flags, and turn on Header Data
    PrintInit ();

	// See if print Header
	if ( ReportOptions & 0x01 )
	{
		// Turn on Printer Header
		PROPT |= OPT1MASK;
	}

	// See if Acquirer never settled, Draw Line and need extra Line Feeds
	if ( ReportOptions & 0x02 )
	{
		// Turn on Printer Option byte 3 so it will Draw line and add Line Feeds
		PROPT |= OPT2MASK;
	}

	// See if time for a Form Feed
	if ( ReportOptions & 0x04 )
	{
		// Turn on Printer Option byte 3 so it will do a formfeed
		PROPT |= OPT3MASK;
	}

	// Print the Receipt
	PrintDriver ( RP_SETTLE_SUM, NULL );

	// Close the Printer
	SDK_PrinterClose(  );

	// Display appropriate printer error.
	if ( PaperOutError == PrinterErrorFlags )
	{
		ShowErrMsg( PaperOut );
		chRetval = 0;
	}

	else if ( PrinterOverheatError == PrinterErrorFlags )
    {
		ShowErrMsg( PrinterOverheat );
		chRetval = 0;
    }

	else if ( OtherPrintError == PrinterErrorFlags )
	{
		ShowErrMsg( PrintError );
		chRetval = 0;
	}

	else
	{
		// Success; no printer problem
		chRetval = 1;
	}

	return chRetval;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints a list of available Hosts
//!
//! \return
//!     None
//!
//! \note
//!		This Procedure uses TRINP to hold the list of Host Names in printable
//!     format.
//-----------------------------------------------------------------------------
extern void PrintHostList( void )
{
	char *pHost;
	UBYTE length;

	// See if Printer is available
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller
		return;
	}

	// Initialize printer PCL data and flags
    PrintInit ();

	// Get Host List data in TRINP structure
	HostList ();

	// Print the Host List Header - "AVAILABLE HOSTS\n\n"
	GetMsg( Msg_AvailHosts, CSTMSG );
	length = StrLn( CSTMSG, sizeof( CSTMSG ) );
	CSTMSG[length++] = '\n';
	CSTMSG[length++] = '\n';
	CSTMSG[length++] = 0;

	SDK_PrintString( CSTMSG );

	// Initialize pointer to first host group
	pHost = ( char * ) &TRINP;

	// Print the line
	SDK_PrintString( pHost );

	// Flush the print buffer with a form-feed
	if ( PrinterOK == PrinterErrorFlags )
		SDK_PrintString( "\f" );

	// Close the Printer
	SDK_PrinterClose(  );

	// If printer error was reported then display error here. 
	if ( PrinterOK != PrinterErrorFlags )
		DisplayPrintError ();

	// Clear the TRINP structure
	memset( ( UBYTE * ) & TRINP, 0, TRSZE );
}

