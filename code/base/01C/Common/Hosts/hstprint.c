//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             hstprint.c
//      Host receipt software module
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
#include "prtdef.h"
#include "util.h"
#include "tables.h"
#include "chkprinter.h"
#include "findtran.h"
#include "totals.h"
#include "journal.h"
#include "hstprint.h"
#include "prtdrv.h"
#include "cvtamt.h"

//=============================================================================
// External variables / declarations
//=============================================================================
//! Custom Function vector Pointers
extern struct FLDDEF *(*PRVECT[8])( void );


//=============================================================================
// Private defines and typedefs
//=============================================================================
// Definitions for 'TipCash' flag
//! Tip Processing Enabled
#define TIP_YES     0x01
//! Cash Back Enabled
#define CASH_YES    0x02


//=============================================================================
// Private function declarations
//=============================================================================
static void IssuerReceiptInternals( void );
static struct FLDDEF *TotalAmount( void );
static struct FLDDEF *TotalCount( void );
static struct FLDDEF *CustTipTotal( void );
static struct FLDDEF *CustTrnTotal( void );
static struct FLDDEF *CustCashTotal( void );
static void PrtSumTipCash( void );
static void SetTipCash( void );
static void PrintCashierRecord( UWORD JrnIdent );
static void PrintCashierError( void );
static void CashierIssuerReceipt( UBYTE ReportOpts );
static Bool PreparePrinter( void );
static void PrnClose( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
//! 1 - Tip, 2 - Cash Back, 0 - Neither
static UBYTE TipCash;
//! Amount Buffer 2
static char AMTBUF2[13];
static UBYTE TIPTOT[S_TRTOTAM];
static UBYTE GNDTOT[S_TRTOTAM];
//! Totals for all Cashiers in a Till
static UBYTE ALLTIP[S_TRTOTAM];
static UBYTE ALLTOT[S_TRTOTAM];
static UBYTE CASHTOT[S_TRTOTAM];
//! Net Count
static UBYTE TMPCNT[2];
//! Net Amount
static UBYTE TMPAMT[S_TRTOTAM];
//! Next Cashier
static UBYTE NXTCSHR[S_TRCASHIER];
//! Save Cashier
static UBYTE SAVCASHIER[S_TRCASHIER];
//! Indicator for Till Summary Report
static Bool TILLFLAG;
//! Print Lines indicator flag
static Bool PRLINES;
static Bool TotalsPrinted;
//! Pointer to ISTAB record
static struct issuer_rec *pIssPtr;

//! Journal Select Default
static const struct jrnfind_rec JSDEF = {
    //! Ident
	0x0000,
    //! Transaction Key
	0xff,
    //! All Acquirers
	0x00,
    //! All Issuers
	0x00,
    //! All Invoices
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    //! Status Bits On
	0x00,
    //! Status Bits 2 On
	0x00,
    //! Status Bits Off
	TS1_REV + TS1_SETL,
    //! Status Bits 2 Off
	0x00,
    //! All Records
	0xff,
    //! All Amounts
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    //! Acquirer to Ignore
	0x00,
    //! All Cashiers
	{0x00, 0x00, 0x00, 0x00},
    //! Shift Number
	0x00
};

static const struct FLDDEF TotalAmtDef = {
    //! ADDRESS OR OFFSET OF THE FIELD
	TMPAMT,
    //! LENGTH OF THE FIELD
	sizeof( TMPAMT ),
    //! ADDRESSING TYPE OF THE FIELD
	A_ACT,
    //! EDIT TYPE FOR THE FIELD
	E_AMT3,
    //! FIELD TYPE
	F_AMT,
    //! TEST TYPE FOR THE FIELD
	T_NULL
};

static const struct FLDDEF TotalCntDef = {
    //! ADDRESS OR OFFSET OF THE FIELD
	TMPCNT,
    //! LENGTH OF THE FIELD
	sizeof( TMPCNT ),
    //! ADDRESSING TYPE OF THE FIELD
	A_ACT,
    //! EDIT TYPE FOR THE FIELD
	E_BCD,
    //! FIELD TYPE
	F_NUM,
    //! TEST TYPE FOR THE FIELD
	T_NULL
};

static const struct FLDDEF TipTotalDef = {
    //! ADDRESS OR OFFSET OF THE FIELD
	TIPTOT,
    //! LENGTH OF THE FIELD
	S_TRTOTAM,
    //! ADDRESSING TYPE OF THE FIELD
	A_ACT,
    //! EDIT TYPE FOR THE FIELD
	E_AMT2,
    //! FIELD TYPE
	F_AMT,
    //! TEST TYPE FOR THE FIELD
	T_NULL
};

static const struct FLDDEF TrnTotalDef = {
    //! ADDRESS OR OFFSET OF THE FIELD
	GNDTOT,
    //! LENGTH OF THE FIELD
	S_TRTOTAM,
    //! ADDRESSING TYPE OF THE FIELD
	A_ACT,
    //! EDIT TYPE FOR THE FIELD
	E_AMT3,
    //! FIELD TYPE
	F_AMT,
    //! TEST TYPE FOR THE FIELD
	T_NULL
};

static const struct FLDDEF CashTotalDef = {
    //! ADDRESS OR OFFSET OF THE FIELD
	CASHTOT,
    //! LENGTH OF THE FIELD
	S_TRTOTAM,
    //! ADDRESSING TYPE OF THE FIELD
	A_ACT,
    //! EDIT TYPE FOR THE FIELD
	E_AMT2,
    //! FIELD TYPE
	F_AMT,
    //! TEST TYPE FOR THE FIELD
	T_NULL
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Prints a summary of Issuers
//!
//! \return
//!     None
//!
//! \note
//!     Called when caller does not have physical control of the printer.
//-----------------------------------------------------------------------------
extern void IssuerReceipt( void )
{
	// Reset the Totals Printed Flag
	TotalsPrinted = False;

	// Print a summary of Issuers
	IssuerReceiptInternals(  );

	// Check if Totals printed
	if ( TotalsPrinted )
	{
		// Flush the print buffer with a form-feed
		if ( PrinterOK == PrinterErrorFlags )
			PrintFormFeed ();
	}

	PrnClose();
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints a list of Transactions
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void PrintList( void )
{
	static UWORD Ident;
    static struct trandata_rec TrnBuf;		// journal record

	// Clear Report Option Flags
	RPTOPTS = 0x00;

	// Set the Cash/Tip flag
	SetTipCash(  );

	// Lock the Journal
	JournalLock ();

	// Clear work area for Tip Total
	memset( TIPTOT, 0, S_TRTOTAM );

	// Clear work area for Grand Total
	memset( GNDTOT, 0, S_TRTOTAM );

	// Clear work area for Cash Total
	memset( CASHTOT, 0, S_TRTOTAM );

	// Set up Default Record
	memcpy( &JSREC, &JSDEF, JSSZE );

	// For this Acquirer
	JSREC.JSAQID = TRINP.TRAQID;

	// Shift Number to use (0 = All)
	JSREC.JSSHIFT = TRINP.TRSHIFT;

	// See if we can find first record
	Ident = JournalSelFirst ();

	// See if any entries in the Journal
	if ( 0x0000 == Ident )
	{
		// No; Release Journal
		JournalRelease ();

		// Notify user: "NO BATCH TOTALS"
		ShowErrMsg( NoBatchTotals );

		// Return to caller
		return;
	}

	// See if Printer is available
	if ( !PreparePrinter() ) 
    {
		// No; Release Journal
		JournalRelease ();

		// Return to caller
        return;
    }

	// Check for All Acquirers
	if ( 0 == TRINP.TRAQID )
	{
		// Yes; set Option 3 for All Acquirers
		PROPT |= OPT3MASK;
	}

	// Is there Cash Back?
	if ( TipCash & CASH_YES )
	{
		// Yes; set Option 2 for Cash Back
		PROPT |= OPT2MASK;
	}

	// Set up Tip Total Custom Routine
	PRVECT[0] = CustTipTotal;

	// Set up Transaction Total Custom Routine
	PRVECT[1] = CustTrnTotal;

	// Set up Cash Total Custom Routine
	PRVECT[4] = CustCashTotal;

	// Loop till done
	while ( True )
	{
		// Read the record that was found
		JournalRead ( Ident, &TrnBuf );

		// Calculate Tip Total
		PrtSumTipCash(  );

		// Print the Transaction (Audit)
		PrintDriver( RP_AUDIT, &TrnBuf );

		// Select the next record
		Ident = JournalSelNext ( Ident );

		// See if found in the Journal
		if ( 0x0000 == Ident )
		{
			// No; exit the loop
			break;
		}

		// See if opened printer is still functional.
		if ( PrinterOK != PrinterErrorFlags )
		{
			break;
		}

	}							// End of while(True).

	// Release Journal
	JournalRelease ();

	// Set Option 1 for Totals
	PROPT |= OPT1MASK;

	// Print the Totals if no printer errors.
	if ( PrinterOK == PrinterErrorFlags )
	{
		PrintDriver( RP_AUDIT, NULL );

		if ( PrinterOK == PrinterErrorFlags )
		{
			// Flush print buffers
			FlushBuffers( 0 );

			// Print the Issuer Receipt
			if ( PrinterOK == PrinterErrorFlags )
			{
				IssuerReceiptInternals(  );

				// Flush the print buffer with a form-feed
				if ( PrinterOK == PrinterErrorFlags )
					PrintFormFeed ();
			}
		}
	}

	// Clear Report Option Flags
	RPTOPTS = 0x00;

    PrnClose();
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints Till Report
//!
//! \return
//!     None
//!
//! \note
//!     As the global input TRINP.TRTILLNO - Till to print.
//-----------------------------------------------------------------------------
extern void PrintTill( void )
{
	// Indicate to print the Till Summary Report
	TILLFLAG = True;

	// Totals only
	RPTOPTS = RPT_SUMRY;

	// Print Totals
	PrintCashierAudit(  );

	// Should we print the Till Summary
	if ( TILLFLAG )
	{
		// Yes; transfer the Totals

		// Tip Totals
		memcpy( TIPTOT, ALLTIP, S_TRTOTAM );

		// Grand Totals
		memcpy( GNDTOT, ALLTOT, S_TRTOTAM );

		// Make sure Cashier is Null
		memset( TRINP.TRCASHIER, 0, S_TRCASHIER );

		// Cashier Totals
		if ( PrinterOK == PrinterErrorFlags )
			CashierIssuerReceipt( RPT_SUMRY );

		// Flush the print buffer with a form-feed
		if ( PrinterOK == PrinterErrorFlags )
			PrintFormFeed ();
	}

	// Close the Printer
	PrnClose();
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints Cashier Audit Report
//!
//! \return
//!     None
//!
//! \note
//!     On Entry as global inputs:<br>
//!       TRINP.TRCASHIER - Cashier to print (0=All)<br>
//!       TRINP.TRSHIFT   - Shift number to use (0=All)<br>
//!       RPTOPTS         - Report Options flag<br>
//!         0 - Detail,<br>
//!         1 - Summary,<br>
//!         2 - Unadjusted.
//-----------------------------------------------------------------------------
extern void PrintCashierAudit( void )
{
    UWORD JrnIdent;

	// Reset the Totals Printed Flag
	TotalsPrinted = False;

	// See if Printer is available
	if ( !PreparePrinter() ) return;

	// Clear work areas for all Cashiers in 1 Till

	// Clear all Tips
	memset( ALLTIP, 0, S_TRTOTAM );

	// Clear all Totals
	memset( ALLTOT, 0, S_TRTOTAM );

	// Save the Cashier
	memcpy( SAVCASHIER, TRINP.TRCASHIER, S_TRCASHIER );

	// Is current Cashier Null?
	if ( NullComp( ( char * ) TRINP.TRCASHIER, S_TRCASHIER ) )
	{
		// Yes; display "SCANNING BATCH/PLEASE WAIT"
		ShowInfoMsg( ScanningBatch, PleaseWait );
	}

	// Do until all Cashiers done
	do
	{
		// Lock the Journal
		JournalLock ();

		// Set up Default Record
		SetDefaultSelection ();

		// Status Bits Off
		JSREC.JSSTATOFF = TS1_REV + TS1_SETL;

		// For this Acquirer
		JSREC.JSAQID = TRINP.TRAQID;

		// If something entered then only find this Cashier
		memcpy( JSREC.JSCASHIER, SAVCASHIER, S_TRCASHIER );

		// Shift Number to use (0 = All)
		JSREC.JSSHIFT = TRINP.TRSHIFT;

		// Print Unadjusted Transactions only?
		if ( RPT_UNADJ == RPTOPTS )
		{
			// Yes; Void and Tip Entered flags must be off
			JSREC.JSSTATOFF |= TS1_VOID + TS1_TIPENT;
		}

		// Set line count to first time value
		PRLINES = True;

		// Reset next Cashier to FF
		memset( NXTCSHR, 0xFF, S_TRCASHIER );

		// Clear work area for Tip Total
		memset( TIPTOT, 0, S_TRTOTAM );

		// Clear work area for Grand Total
		memset( GNDTOT, 0, S_TRTOTAM );

		// Clear work area for Cash Back Total
		memset( CASHTOT, 0, S_TRTOTAM );

		// Set the Cash/Tip flag
		SetTipCash(  );

		// Initialize printer PCL data and flags
		PrintInit(  );

		// Is there Cash Back?
		if ( TipCash & CASH_YES )
		{
			// Yes; set Option 2 for Cash Back
			PROPT |= OPT2MASK;
		}

		// Unadjusted Report?
		if ( RPT_UNADJ == RPTOPTS )
		{
			// Yes; set Option 3 for Unadjusted Report
			PROPT |= OPT3MASK;
		}

		// Set up Tip Total Custom Routine
		PRVECT[2] = CustTipTotal;

		// Set up Transaction Total Custom Routine
		PRVECT[3] = CustTrnTotal;

		// Set up Cash Total Custom Routine
		PRVECT[4] = CustCashTotal;

		// See if we can find first record
		JrnIdent = JournalSelFirst ();

		// See if any entries in the Journal
		if ( 0x0000 == JrnIdent )
		{
			// No; Release Journal
			JournalRelease ();

			// Notify user
			PrintCashierError(  );

			// Exit the loop
			break;
		}

		// Do until no more Journal records
		do
		{
			// Print a Cashier Record
			PrintCashierRecord( JrnIdent );

			// Select the next record
			JrnIdent = JournalSelNext ( JrnIdent );

		}
		while ( ( 0x0000 != JrnIdent ) &&
				( PrinterOK == PrinterErrorFlags ) );

		// Release Journal
		JournalRelease ();

		// Exit now if the printer has a problem.
		if ( PrinterOK != PrinterErrorFlags )
			break;

		// Is Current Cashier Null?
		if ( !NullComp( ( char * ) TRINP.TRCASHIER, S_TRCASHIER ) )
		{
			// No; is it first time value?
			if ( PRLINES )
			{
				// Yes; nothing found to print, notify user
				if ( PrinterOK == PrinterErrorFlags )
					PrintCashierError(  );

				// Exit the loop
				break;
			}

			// Summary Report?
			if ( RPT_SUMRY != RPTOPTS )
			{
				// No; set Option 1 for Totals
				PROPT |= OPT1MASK;

				// Print Totals
				if ( PrinterOK == PrinterErrorFlags )
				{
					PrintDriver( RP_CASHIER, NULL );

					// Totals have been printed
					TotalsPrinted = True;
				}
				else
					break;
			}

			// Get Option flag and print Cashier Issuer Receipt
			if ( PrinterOK == PrinterErrorFlags )
				CashierIssuerReceipt( RPTOPTS );
			else
				break;
		}

		// Any more Cashiers?
		if ( 0xff == NXTCSHR[0] )
		{
			// No; is it first time value
			if ( PRLINES )
			{
				if ( PrinterOK == PrinterErrorFlags )
					PrintCashierError(  );
			}

			// Exit the loop
			break;
		}

		// Move Next Cashier to the Current Cashier
		memcpy( TRINP.TRCASHIER, NXTCSHR, S_TRCASHIER );

	}
	while ( PrinterOK == PrinterErrorFlags );

	// Check if Totals printed
	if ( TotalsPrinted )
	{
		// Flush the print buffer with a form-feed
		if ( PrinterOK == PrinterErrorFlags )
			PrintFormFeed ();
	}

	// Clear Report Option flags
	RPTOPTS = 0x00;

	// Close the Printer
	PrnClose();
}


//-----------------------------------------------------------------------------
//! \brief
//!     Displays message that printer had an error
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void DisplayPrintError( void )
{
	// Display appropriate printer error.
	if ( PaperOutError == PrinterErrorFlags )
		ShowErrMsg( PaperOut );

 	else if ( PrinterOverheatError == PrinterErrorFlags )
		ShowErrMsg( PrinterOverheat );

	else if ( OtherPrintError == PrinterErrorFlags )
		ShowErrMsg( PrintError );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sends one line feed to the printer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void PrintLineFeed( void )
{
	if ( orvOK == SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) )
	{
		SDK_PrintString( " \n" );

		// Wait till printing is done
		while ( SDK_PrinterBusy( ) && ( 0x0E != TCONF.TCPRINTER ) )
		{
			// Give up the CPU
			SDK_RelinqCPU(  );
		}

		SDK_PrinterClose(  );
	}
}


//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Prints a summary of Issuers
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void IssuerReceiptInternals( void )
{
	// See if Printer is available
	if ( !PreparePrinter() ) return;

	// Set the Cash/Tip flag
	SetTipCash(  );

	// Set up Total Count Custom Routine
	PRVECT[0] = TotalCount;

	// Set up Total Amount Custom Routine
	PRVECT[1] = TotalAmount;

	// Is there Cash Back?
	if ( TipCash & CASH_YES )
	{
		// Yes; set Option 2 for Cash Back
		PROPT |= OPT2MASK;
	}

	// Loop through the Issuers
    for ( pIssPtr = &ISTAB[0]; pIssPtr < &ISTAB[ISMAX]; pIssPtr++ )
	{
		// See if Table Populated
        if ( pIssPtr->ISNUM )
		{
			// Yes; lock the Journal
			JournalLock ();

			// Set up Default Record
			memcpy( &JSREC, &JSDEF, JSSZE );

			// For this Acquirer
			JSREC.JSAQID = TRINP.TRAQID;

			// If Reprint do not ignore Settled Transaction
			if ( TRINP.TRREPRINT )
			{
				// Select Settled Transactions
				JSREC.JSSTATOFF &= ~TS1_SETL;
			}

			// Get Issuer Id; store in Selection Record
            JSREC.JSISID = pIssPtr->ISID;

			// Shift Number to use (0 = All)
			JSREC.JSSHIFT = TRINP.TRSHIFT;

			// Calculate Totals for this Issuer
			CalcTotals_Jsrec ();

			// See if Totals Exist
			if ( !NullComp( ( char * ) &TERMTOTALS, TOTSZE ) )
			{

				// Yes; get Issuer Table pointer
				JRREC.TRISPTR = pIssPtr;

				// Print the Totals
				PrintDriver( RP_ISSUER, &JRREC );

				// Totals have been printed
				TotalsPrinted = True;
			}

			// Release Journal
			JournalRelease ();
		}
	}

	// Lock the Journal
	JournalLock ();

	// Set up Default Record
	memcpy( &JSREC, &JSDEF, JSSZE );

	// Print for all Issuers

	// For this Acquirer
	JSREC.JSAQID = TRINP.TRAQID;

	// If Reprint do not ignore Settled Transaction
	if ( TRINP.TRREPRINT )
	{
		// Select Settled Transactions
		JSREC.JSSTATOFF &= ~TS1_SETL;
	}

	// Calculate Totals for all Issuers
	CalcTotals_Jsrec ();

	// Release Journal
	JournalRelease ();

	// See if Totals Exist
	if ( !NullComp( ( char * ) &TERMTOTALS, TOTSZE ) )
	{
		// Yes; set Option 1 for Grand Totals
		PROPT |= OPT1MASK;

		// Print the Totals
		PrintDriver( RP_ISSUER, NULL );

		// Totals have been printed
		TotalsPrinted = True;
	}
	else
	{
		// No; notify user: "NO BATCH TOTALS"
		ShowErrMsg( NoBatchTotals );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Calculates Total Amount
//!
//! \return
//!     Pointer to new Field Definition
//-----------------------------------------------------------------------------
static struct FLDDEF *TotalAmount( void )
{
	// Calculate Total Amount
	SubAmt( TMPAMT, TERMTOTALS.TOTBASIC.TOTDBAMT,
			TERMTOTALS.TOTBASIC.TOTCRAMT );

	// Return pointer to new Field Definition
	return ( ( struct FLDDEF * ) &TotalAmtDef );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Calculates Total Count
//!
//! \return
//!     Pointer to new Field Definition
//-----------------------------------------------------------------------------
static struct FLDDEF *TotalCount( void )
{
	// Calculate Total Count
	AddCnt( TMPCNT, TERMTOTALS.TOTBASIC.TOTCRCNT,
			TERMTOTALS.TOTBASIC.TOTDBCNT );

	// Return pointer to new Field Definition
	return ( ( struct FLDDEF * ) &TotalCntDef );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Calculates Tip Total Amount
//!
//! \return
//!     Pointer to new Field Definition
//-----------------------------------------------------------------------------
static struct FLDDEF *CustTipTotal( void )
{
	// Return pointer to new Field Definition
	return ( ( struct FLDDEF * ) &TipTotalDef );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Calculates Transaction Total Amount
//!
//! \return
//!     Pointer to new Field Definition
//-----------------------------------------------------------------------------
static struct FLDDEF *CustTrnTotal( void )
{
	// Return pointer to new Field Definition
	return ( ( struct FLDDEF * ) &TrnTotalDef );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Calculates Cash Total Amount
//!
//! \return
//!     Pointer to new Field Definition
//-----------------------------------------------------------------------------
static struct FLDDEF *CustCashTotal( void )
{
	// Return pointer to new Field Definition
	return ( ( struct FLDDEF * ) &CashTotalDef );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Calculates Tip Total
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PrtSumTipCash( void )
{
	// Tip or Cash?
	if ( TipCash )
	{
		// Yes; get the transaction from the journal record read in
		SecondEvent.TRNKEY = JRREC.TRKEY;

		// Look for the transaction
		FindTran ();

		// See if Tips are done on this type of transaction
		if ( !( SecondEvent.TRNOPT1 & T1_NOTIP ) )
		{
			// Yes; prepare Tip Amount
			CvtAmt( AMTBUF2, JRREC.TRTIP );

			// Don't add to Totals if VOID
			if ( !( JRREC.TRSTATUS[0] & TS1_VOID ) )
			{
				if ( SecondEvent.TRNOPT1 & T1_CREDIT )
				{
					// Save Tip/Cash Total
					SubAmt( TIPTOT, TIPTOT, JRREC.TRTIP );

					// Save Gnd Total
					SubAmt( GNDTOT, GNDTOT, JRREC.TRTOTAM );

					// Save Tip/Cash Total
					SubAmt( ALLTIP, ALLTIP, JRREC.TRTIP );

					// Save Gnd Total
					SubAmt( ALLTOT, ALLTOT, JRREC.TRTOTAM );

					// Save Cash Total
					SubAmt( CASHTOT, CASHTOT, JRREC.TRCASHBK );
				}
				else
				{
					// Save Tip/Cash Total
					AddAmt( TIPTOT, TIPTOT, JRREC.TRTIP );

					// Save Gnd Total
					AddAmt( GNDTOT, GNDTOT, JRREC.TRTOTAM );

					// Save Tip/Cash Total
					AddAmt( ALLTIP, ALLTIP, JRREC.TRTIP );

					// Save Gnd Total
					AddAmt( ALLTOT, ALLTOT, JRREC.TRTOTAM );

					// Save Cash Total
					AddAmt( CASHTOT, CASHTOT, JRREC.TRCASHBK );
				}
			}
		}
		else
		{
			// No; clear the Tip/Cash Amount to spaces
			memset( AMTBUF2, ' ', sizeof( AMTBUF2 ) );
		}
	}
	else
	{
		// No; clear the Tip/Cash Amount to spaces
		memset( AMTBUF2, ' ', sizeof( AMTBUF2 ) );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sets 'TipCash' flag
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SetTipCash( void )
{
	// Option 0 - Neither Tip nor Cash Back
	TipCash = 0x00;

	// See if Restaurant Mode
	if ( TCONF.TCOPT1 & TC1_REST )
	{
		// Yes; Option 1 - Print Tip Amount
		TipCash |= TIP_YES;
	}

     /* See if any Issuers have Cash Back */                                          
     for ( pIssPtr = &ISTAB[0]; pIssPtr < &ISTAB[ISMAX]; pIssPtr++ )            
     {                                                                                
                                                                                      
             /* Yes; Cash Back? */                                                    
             if ( (pIssPtr->ISOPT3) & IS3_CASHAMT )                     
             {                                                                        
                 /* Yes; Option 2 - Print Cash Back Amount */                         
                 TipCash |= CASH_YES;                                                 
                                                                                      
                 /* Exit the loop */                                                  
                 break;                                                               
             }                                                                        
     }                                                                                
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints Cashier Record
//!
//! \param
//!		[in]JrnIdent			Journal record identifier
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PrintCashierRecord( UWORD JrnIdent )
{
	BYTE Check;

	// Read the record that was found
	JournalRead ( JrnIdent, &JRREC );

	// Printing Till Report?
	if ( PRTTILL == TRINP.TRKEY )
	{
		// Yes; check if Till matches Journal Till
		if ( memcmp( TRINP.TRTILLNO, JRREC.TRTILLNO, S_TRTILLNO ) )
		{
			// No; return to caller
			return;
		}
	}

	// Check if Current Cashier matches Journal Cashier
	Check = memcmp( TRINP.TRCASHIER, JRREC.TRCASHIER, S_TRCASHIER );

	if ( Check )
	{
		// No; see if Journal Cashier less than Current Cashier
		if ( Check > 0 )
		{
			// Yes; return to caller
			return;
		}

		// Check if Next Cashier less than Journal Cashier
		if ( memcmp( NXTCSHR, JRREC.TRCASHIER, S_TRCASHIER ) < 0 )
		{
			// Yes; return to caller
			return;
		}

		// Unadjusted Report?
		if ( RPT_UNADJ == RPTOPTS )
		{
			// Yes; get the transaction from the journal record read 
			// in
			SecondEvent.TRNKEY = JRREC.TRKEY;

			// Look for the transaction
			FindTran ();

			// See if a Credit
			if ( SecondEvent.TRNOPT1 & T1_CREDIT )
			{
				// Yes; return to caller
				return;
			}

			// Is Tip Null?
			if ( !NullComp( ( char * ) JRREC.TRTIP, S_TRTIP ) )
			{
				// No; don't print records with Tips, return to caller
				return;
			}
		}

		// Make Journal Cashier the Next Cashier
		memcpy( NXTCSHR, JRREC.TRCASHIER, S_TRCASHIER );

		// Return to caller
		return;
	}

	// Is Current Cashier Null?
	if ( NullComp( ( char * ) TRINP.TRCASHIER, S_TRCASHIER ) )
	{
		// Yes; don't print Null Cashiers, return to caller
		return;
	}

	// Print Summary Report?
	if ( RPT_SUMRY == RPTOPTS )
	{
		// Yes; get Tip Totals
		PrtSumTipCash(  );

		// Nothing to print
		PRLINES = False;

		// Return to caller
		return;
	}

	// Unadjusted Report?
	if ( RPT_UNADJ == RPTOPTS )
	{
		// Yes; get the transaction from the journal record 
		// read in
		SecondEvent.TRNKEY = JRREC.TRKEY;

		// Look for the transaction
		FindTran ();

		// See if a Credit
		if ( SecondEvent.TRNOPT1 & T1_CREDIT )
		{
			// Yes; return to caller
			return;
		}

		// Is Tip Null?
		if ( !NullComp( ( char * ) JRREC.TRTIP, S_TRTIP ) )
		{
			// No; don't print records with Tips, return to caller
			return;
		}
	}

	// Get Tip Totals
	PrtSumTipCash(  );

	// Print Cashier Record
	if ( PrinterOK == PrinterErrorFlags )
		PrintDriver( RP_CASHIER, &JRREC );

	// Nothing to Print
	PRLINES = False;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Reports Print Cashier error to user
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PrintCashierError( void )
{
	// Notify user: "NO TRANSACTIONS"
	ShowErrMsg( NoTransactions );

	// Indicate to not print the Till Summary Report
	TILLFLAG = False;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints a summary of Issuers for a Cashier
//!
//! \param
//!		[in]ReportOpts			Report options
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void CashierIssuerReceipt( UBYTE ReportOpts )
{
	// Save option flag
	RPTOPTS = ReportOpts;

	// See if Printer is available
	if ( !PreparePrinter() ) return;

	// Set the Cash/Tip flag
	SetTipCash(  );

	// Is it the Summary Report?
	if ( RPT_SUMRY == RPTOPTS )
	{
		// Yes; is there Cash Back?
		if ( TipCash & CASH_YES )
		{
			// Yes; set Option 2 for Cash Back
			PROPT |= OPT2MASK;
		}

		// Set Option 4 for Cashier to Print
		PROPT |= OPT4MASK;
	}

	// Printing Till Report?
	if ( PRTTILL == TRINP.TRKEY )
	{
		// Yes; set Option 5 for Till to Print
		PROPT |= OPT5MASK;
	}

	// Set up Total Count Custom Routine
	PRVECT[0] = TotalCount;

	// Set up Total Amount Custom Routine
	PRVECT[1] = TotalAmount;

	// Set up Tip Total Custom Routine
	PRVECT[2] = CustTipTotal;

	// Set up Transaction Total Custom Routine
	PRVECT[3] = CustTrnTotal;

	// Set up Cash Total Custom Routine
	PRVECT[4] = CustCashTotal;

	// Loop through the Issuers
	for ( pIssPtr = &ISTAB[0]; pIssPtr < &ISTAB[ISMAX]; pIssPtr++ )
	{
		// See if Table Populated
		if ( pIssPtr->ISNUM )
		{
			// Yes; lock the Journal
			JournalLock ();

			// Set up Default Record
			SetDefaultSelection ();

			// Status Bits Off
			JSREC.JSSTATOFF = TS1_REV + TS1_SETL;

			// For this Acquirer
			JSREC.JSAQID = TRINP.TRAQID;

			// For this Cashier
			memcpy( JSREC.JSCASHIER, TRINP.TRCASHIER, S_TRCASHIER );

			// Shift Number to use (0 = All)
			JSREC.JSSHIFT = TRINP.TRSHIFT;

			// Print Unadjusted Transactions only?
			if ( RPT_UNADJ == RPTOPTS )
			{
				// Yes; Void and Tip Entered flags must be off
				JSREC.JSSTATOFF |= TS1_VOID + TS1_TIPENT;
			}

			// Get Issuer Id; store in Selection Record
			JSREC.JSISID = pIssPtr->ISID;

			// Calculate Totals for this Issuer
			CalcTotals_Jsrec ();

			// See if Totals Exist
			if ( !NullComp( ( char * ) &TERMTOTALS, TOTSZE ) )
			{
				// Yes; get Issuer Table pointer
				TRINP.TRISPTR = pIssPtr;

				// Print the Totals
				if ( PrinterOK == PrinterErrorFlags )
				{
					PrintDriver( RP_ISSUER, &TRINP );

					// Totals have been printed
					TotalsPrinted = True;
				}

			}

			// Release Journal
			JournalRelease ();
		}
	}

	// Lock the Journal
	JournalLock ();

	// Set up Default Record
	SetDefaultSelection ();

	// Status Bits Off
	JSREC.JSSTATOFF = TS1_REV + TS1_SETL;

	// Print for all Issuers

	// For this Acquirer
	JSREC.JSAQID = TRINP.TRAQID;

	// For this Cashier
	memcpy( JSREC.JSCASHIER, TRINP.TRCASHIER, S_TRCASHIER );

	// Print Unadjusted Transactions only?
	if ( RPT_UNADJ == RPTOPTS )
	{
		// Yes; Void and Tip Entered flags must be off
		JSREC.JSSTATOFF |= TS1_VOID + TS1_TIPENT;
	}

	// Calculate Totals for all Issuers
	CalcTotals_Jsrec ();

	// Release Journal
	JournalRelease ();

	// See if Totals Exist
	if ( !NullComp( ( char * ) &TERMTOTALS, TOTSZE ) )
	{
		// Yes; set Option 1 for Grand Totals
		PROPT |= OPT1MASK;

		// Print the Totals
		if ( PrinterOK == PrinterErrorFlags )
		{
			PrintDriver( RP_ISSUER, &TRINP );

			// Totals have been printed
			TotalsPrinted = True;
		}

		// See if Summary Print?
		if ( RPT_SUMRY == RPTOPTS )
		{
			// Yes; is there Cash Back?
			if ( TipCash & CASH_YES )
			{
				// Yes; set Option 2 for Cash Back
				PROPT |= OPT2MASK;
			}

			// Set Option 1 for Printing Totals
			PROPT |= OPT1MASK;

			// Print the Totals
			if ( PrinterOK == PrinterErrorFlags )
				PrintDriver( RP_CASHIER, NULL );

			// Totals have been printed
			TotalsPrinted = True;
		}
	}
	else
	{
		// No; notify user: "NO BATCH TOTALS"
		ShowErrMsg( NoBatchTotals );
	}
}



//-----------------------------------------------------------------------------
//! \brief
//!     Checks printer and initializes it
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
static Bool PreparePrinter( void )
{
	// See if Printer is available
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller
		return False;
	}

	// Initialize printer PCL data and flags
	PrintInit(  );

	return True;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Closes printer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PrnClose( void )
{
	SDK_PrinterClose(  );

	// If printer error was reported then display error here. 
	if ( PrinterOK != PrinterErrorFlags )
		DisplayPrintError(  );
}

